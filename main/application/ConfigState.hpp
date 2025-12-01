#ifndef CONFIG_STATE_HPP
#define CONFIG_STATE_HPP

#include "AppState.hpp"
#include "../net/wifi/WifiAp.hpp"
#include "../http/HttpServer.hpp"

typedef struct dns_server_handle *dns_server_handle_t;

class ConfigState: public AppState
{
    public:
        ConfigState();
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override
        {
            return "Config";
        }

        AppStateType getType() const override
        {
            return CONFIG;
        }

    private:
        WifiAp *wifi_ap_;
        HttpServer *http_server_;
        dns_server_handle_t dns_server_;
};

#endif
