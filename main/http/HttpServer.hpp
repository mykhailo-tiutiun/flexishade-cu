#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

typedef void* httpd_handle_t;

class HttpServer {
    private:
        httpd_handle_t server_;
    public:
        HttpServer();
        ~HttpServer();
};

#endif