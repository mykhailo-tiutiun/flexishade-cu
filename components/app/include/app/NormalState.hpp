#ifndef NORMAL_STATE_HPP
#define NORMAL_STATE_HPP

#include "app/AppState.hpp"
#include "wifi/WifiSta.hpp"

class NormalState: public AppState {
    private:
        WifiSta* wifi_sta_;
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override
        {
            return "Normal";
        }
        AppStateType getType() const override
        {
            return NORMAL;
        }
};

#endif
