#include "io/config/HttpServer.hpp"
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"

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
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.uri_match_fn = httpd_uri_match_wildcard;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    ESP_ERROR_CHECK(httpd_start(&server, &config));

    httpd_register_uri_handler(server, &hello);
    httpd_register_uri_handler(server, &default_url);


    server_ = server;
}

HttpServer::~HttpServer() {
    httpd_stop(server_);
}
