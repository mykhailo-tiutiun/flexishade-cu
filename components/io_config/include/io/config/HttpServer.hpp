#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "io/config/HttpHandler.hpp"
#include <map>
#include <string>
typedef void* httpd_handle_t;

class HttpServer {
    public:
        HttpServer();
        ~HttpServer();
        void start();

        void registerHandler(HttpHandler handler);

        void stop();
    private:
        httpd_handle_t server_;
        std::map<std::string, HttpHandler> handlers_;

        static esp_err_t handle(httpd_req_t *req);
};

#endif
