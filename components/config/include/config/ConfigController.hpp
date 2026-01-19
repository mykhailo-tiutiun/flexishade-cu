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

        HttpResponse getHtmlPage(HttpRequest request);

        HttpResponse getConfigs(HttpRequest request);
        HttpResponse saveConfig(HttpRequest request);

    private:
        HttpServer* http_server_;
        ConfigService* service_;
};

#endif
