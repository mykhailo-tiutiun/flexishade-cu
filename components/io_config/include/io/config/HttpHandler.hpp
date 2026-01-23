#ifndef HTTP_HANDLER_HPP
#define HTTP_HANDLER_HPP

#include "esp_http_server.h"
#include <functional>
#include <string>

struct HttpRequest
{
    std::string uri;
    httpd_method_t method;
    std::string content;
    std::string content_type;
};

struct HttpResponse
{
    std::string content;
    int status;
    std::string content_type;
};

class HttpHandler
{
    public:
        using Handler = std::function<HttpResponse(HttpRequest, void*)>;

        HttpHandler(std::string uri, httpd_method_t method, Handler handler, void* handler_args);

        void handle(httpd_req_t *req);

        const std::string& getUri() const;
        httpd_method_t getMethod() const;

    private:
        std::string uri_;
        httpd_method_t method_;
        Handler handler_;
        void* handler_args_;
};

#endif
