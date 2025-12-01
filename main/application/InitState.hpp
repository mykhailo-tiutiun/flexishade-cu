#ifndef INIT_STATE_HPP
#define INIT_STATE_HPP

#include "AppState.hpp"

class InitState: public AppState {
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override
        {
            return "Init";
        }

        AppStateType getType() const override
        {
            return INIT;
        }
};

#endif
