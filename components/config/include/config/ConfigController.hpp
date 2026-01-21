#ifndef CONFIG_CONTROLLER_HPP
#define CONFIG_CONTROLLER_HPP

#include "config/ConfigService.hpp"
#include "io/config/HttpHandler.hpp"
#include "io/config/HttpServer.hpp"

class ConfigController
{
    public:
        ConfigController(HttpServer* http_server, ConfigService* service);
        void registerHandlers();

    private:
        HttpServer* http_server_;
        ConfigService* service_;

        HttpHandler* getHtmlPageHandler();
        HttpHandler* getConfigsHandler();
        HttpHandler* saveConfigHandler();

        static HttpResponse getHtmlPage(HttpRequest request, void* args);
        static HttpResponse getConfigs(HttpRequest request, void* args);
        static HttpResponse saveConfig(HttpRequest request, void* args);
};

#endif
