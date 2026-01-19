#include "io/config/HttpServer.hpp"
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "http_parser.h"
#include <cstdlib>
#include <functional>
#include <string>

static const char *TAG = "http server";



static esp_err_t captive_portal_handler(httpd_req_t *req) {
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "http://192.168.4.1/");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t default_url = {
    .uri       = "/*",
    .method    = HTTP_GET,
    .handler   = captive_portal_handler,
    .user_ctx  = NULL
};

static esp_err_t hello_get_handler(httpd_req_t *req) {
    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* resp_str = (const char*) req->user_ctx;
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static const httpd_uri_t hello = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = hello_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = (void*)"Hello World!"
};

HttpServer::HttpServer() : server_(nullptr) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    ESP_ERROR_CHECK(httpd_start(&server_, &config));

    httpd_register_uri_handler(server_, &hello);
    httpd_register_uri_handler(server_, &default_url);
}

HttpServer::~HttpServer() {
    httpd_stop(server_);
}

esp_err_t HttpServer::handle(httpd_req_t *req)
{
    auto server = (HttpServer*) req->user_ctx;

    int content_len = req->content_len;
    char* content = (char*)calloc(content_len, sizeof(char));

    httpd_req_recv(req, content, content_len);

    HttpRequest request = {
        .uri = std::string(req->uri),
        .method = http_method(req->method),
        .content = std::string(content, content_len),
    };

    auto handler_it = server->handlers_.find(request.uri);
    if (handler_it != server->handlers_.end()) {
        auto response = handler_it->second.handleData(request);
        httpd_resp_send(req, response.content, response.content);
    }

    return ESP_OK;
}

void HttpServer::registerHandler(HttpHandler handler)
{
    const httpd_uri_t h = {
        .uri = handler.getUri().c_str(),
        .method = handler.getMethod(),
        .handler = handle,
        .user_ctx = (void*)this,
    };

    httpd_register_uri_handler(server_, &h);
}
