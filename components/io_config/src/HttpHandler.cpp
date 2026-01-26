#include "io/config/HttpHandler.hpp"
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_log.h"

static const char* TAG = "http_handler";

HttpHandler::HttpHandler(std::string uri, httpd_method_t method, Handler handler, void* handler_args)
: uri_(uri)
, method_(method)
, handler_(handler)
, handler_args_(handler_args)
{}

esp_err_t HttpHandler::handle(httpd_req_t *req)
{
    int content_len = req->content_len;
    char* content = (char*)calloc(content_len, sizeof(char));

    httpd_req_recv(req, content, content_len);

    HttpRequest request = {
        .uri = std::string(req->uri),
        .method = http_method(req->method),
        .content = std::string(content, content_len),
    };

    free(content);

    ESP_LOGI(TAG, "handle request %s", uri_.c_str());

    auto response = (handler_) (request, handler_args_);

    httpd_resp_set_type(req, response.content_type.c_str());
    httpd_resp_send(req, response.content.c_str(), response.content.size());

    return ESP_OK;
}

const std::string& HttpHandler::getUri() const
{
    return uri_;
}

httpd_method_t HttpHandler::getMethod() const
{
    return method_;
}
