#include "config/ConfigController.hpp"
#include "config/IndexPage.hpp"
#include "http_parser.h"
#include "io/config/HttpHandler.hpp"
#include <string>

ConfigController::ConfigController(HttpServer* http_server, ConfigService* service)
: http_server_(http_server)
, service_(service)
{
}

void ConfigController::registerHandlers()
{
    http_server_->addHandler(getHtmlPageHandler());
    http_server_->addHandler(getConfigsHandler());
    http_server_->addHandler(saveConfigHandler());
}

HttpHandler* ConfigController::getHtmlPageHandler()
{
    auto handler = new HttpHandler("/", HTTP_GET, getHtmlPage, nullptr);
    return handler;
}

HttpHandler* ConfigController::getConfigsHandler()
{
    auto handler = new HttpHandler("/api/config", HTTP_GET, getConfigs, (void*)service_);
    return handler;
}

HttpHandler* ConfigController::saveConfigHandler()
{
    auto handler = new HttpHandler("/api/config/save", HTTP_POST, saveConfig, (void*)service_);
    return handler;
}

HttpResponse ConfigController::getHtmlPage(HttpRequest request, void* args)
{
    HttpResponse response = {
        .content = getIndexHtml(),
        .status = 200,
        .content_type = "text/html",
    };

    return response;
}

HttpResponse ConfigController::getConfigs(HttpRequest request, void* args)
{
    auto service = (ConfigService*) args;
    auto configs_json = service->exportAllToJson();

    HttpResponse response = {
        .content = std::string(cJSON_PrintUnformatted(configs_json)),
        .status = 200,
        .content_type = "application/json",
    };

    return response;
}

HttpResponse ConfigController::saveConfig(HttpRequest request, void* args)
{
    auto service = (ConfigService*) args;
    auto config_json = cJSON_Parse(request.content.data());

    service->importJson("wifi", config_json);

    HttpResponse response = {
        .status = 204,
        .content_type = "application/json",
    };

    return response;
}
