#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include "http/HttpHandler.hpp"
#include <expected>
#include <map>
#include <string>
typedef void* httpd_handle_t;

class HttpServer {
    public:
        HttpServer();
        ~HttpServer();

        std::expected<void, std::string> start();
        std::expected<void, std::string> stop();

        void addHandler(HttpHandler* handler);

        bool isUp() const;

    private:
        httpd_handle_t server_;
        std::map<std::string, HttpHandler*> handlers_;
        bool is_up_;

        void registerAllHandlers();

        static esp_err_t handle(httpd_req_t *req);
};

#endif
