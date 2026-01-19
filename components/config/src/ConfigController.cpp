#include "config/ConfigController.hpp"
#include "io/config/HttpHandler.hpp"

ConfigController::ConfigController(HttpServer* http_server, ConfigService* service)
: http_server_(http_server)
, service_(service)
{
}

void ConfigController::registerHandlers()
{

}



HttpResponse ConfigController::getHtmlPage(HttpRequest request)
{
    HttpResponse response = {
        .content = std::string("Hello config"),
        .status = 200,
    };

    return response;
}

HttpResponse ConfigController::getConfigs(HttpRequest request)
{
    auto configs_json = service_->exportAllToJson();

    HttpResponse response = {
        .content = std::string(cJSON_PrintUnformatted(configs_json)),
        .status = 200,
    };

    return response;
}

HttpResponse ConfigController::saveConfig(HttpRequest request)
{
    auto config_json = cJSON_Parse(request.content.data());

    service_->importJson("wifi", config_json);

    HttpResponse response = {
        .status = 204,
    };

    return response;
}
