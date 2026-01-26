#ifndef NORMAL_ERR_STATE_HPP
#define NORMAL_ERR_STATE_HPP

#include "app/AppState.hpp"

#define RETRY_DELAY_SECONDS 10

class NormalErrState: public AppState {
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override
        {
            return "NormalErr";
        }
        AppStateType getType() const override
        {
            return NORMAL_ERR;
        }
};

#endif
