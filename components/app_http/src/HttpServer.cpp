#include "http/HttpServer.hpp"
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "http_parser.h"
#include <cstdlib>
#include <functional>
#include <string>
#include <expected>

#define ERROR_CHECK(exp, msg) if (exp == ESP_FAIL) { return std::unexpected(msg); }

static const char *TAG = "http_server";

static esp_err_t captive_portal_handler(httpd_req_t *req) {
    httpd_resp_set_status(req, "302 Found");
    httpd_resp_set_hdr(req, "Location", "http://192.168.4.1/");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

HttpServer::HttpServer()
{}

HttpServer::~HttpServer()
{
    stop();
}

std::expected<void, std::string> HttpServer::start()
{
    if (is_up_ == true) {
        return std::unexpected("HttpServer start, unexpected state: up");
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.uri_match_fn = httpd_uri_match_wildcard;

    ERROR_CHECK(httpd_start(&server_, &config), "HttpServer init failed");
    registerAllHandlers();

    is_up_ = true;
    ESP_LOGI(TAG, "Started on port: '%d'", config.server_port);

    return {};
}

std::expected<void, std::string> HttpServer::stop()
{
    if (is_up_ == false) {
        return std::unexpected("HttpServer stop, unexpected state: down");
    }

    is_up_ = false;

    httpd_stop(server_);
    ESP_LOGI(TAG, "Stopped");

    return {};
}

esp_err_t HttpServer::handle(httpd_req_t *req)
{
    auto server = (HttpServer*) req->user_ctx;

    auto handler_it = server->handlers_.find(req->uri);

    if (handler_it != server->handlers_.end()) {
        return handler_it->second->handle(req);
    } else {
        return captive_portal_handler(req);
    }
}

void HttpServer::addHandler(HttpHandler* handler)
{
    handlers_[handler->getUri()] = handler;
}

void HttpServer::registerAllHandlers()
{
    const httpd_uri_t h = {
        .uri = "/*",
        .method = (httpd_method_t)HTTP_ANY,
        .handler = handle,
        .user_ctx = (void*)this,
    };

    httpd_register_uri_handler(server_, &h);
}

bool HttpServer::isUp() const
{
    return is_up_;
}
