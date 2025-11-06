#ifndef CONFIG_STATE_HPP
#define CONFIG_STATE_HPP

#include "AppState.hpp"

class ConfigState: public AppState {
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override;
};

#endif