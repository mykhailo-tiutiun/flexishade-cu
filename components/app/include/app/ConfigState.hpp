#ifndef CONFIG_STATE_HPP
#define CONFIG_STATE_HPP

#include "app/AppState.hpp"
#include "wifi/WifiAp.hpp"
#include "http/HttpServer.hpp"

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
};

#endif
