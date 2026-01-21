#include "io/config/HttpHandler.hpp"
#include "esp_log.h"

static const char* TAG = "http_handler";

HttpHandler::HttpHandler(std::string uri, httpd_method_t method, Handler handler, void* handler_args)
: uri_(uri)
, method_(method)
, handler_(handler)
, handler_args_(handler_args)
{}

HttpResponse HttpHandler::handle(HttpRequest request)
{
    ESP_LOGI(TAG, "handle request %s", uri_.c_str());
    return (handler_) (request, handler_args_);
}

const std::string& HttpHandler::getUri() const
{
    return uri_;
}

httpd_method_t HttpHandler::getMethod() const
{
    return method_;
}
