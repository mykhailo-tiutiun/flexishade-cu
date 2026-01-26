#ifndef CONFIG_ERR_STATE_HPP
#define CONFIG_ERR_STATE_HPP

#include "app/AppState.hpp"

#define RETRY_DELAY_SECONDS 100

class ConfigErrState: public AppState {
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override
        {
            return "ConfigErr";
        }
        AppStateType getType() const override
        {
            return CONFIG_ERR;
        }
};

#endif
