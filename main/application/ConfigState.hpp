#ifndef CONFIG_STATE_HPP
#define CONFIG_STATE_HPP

#include "AppState.hpp"

class SoftAp;

class ConfigState: public AppState {
    private:
        SoftAp *soft_ap_;
    public:
        ConfigState();
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override;
};

#endif