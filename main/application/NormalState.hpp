#ifndef NORMAL_STATE_HPP
#define NORMAL_STATE_HPP

#include "AppState.hpp"

#include "../net/wifi/WifiSta.hpp"
#include "../mqtt/MqttClient.hpp"

class NormalState: public AppState {
    private:
        WifiSta* wifi_sta_;
        MqttClient* mqtt_;
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override;
};

#endif