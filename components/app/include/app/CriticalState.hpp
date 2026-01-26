#ifndef CRITICAL_STATE_HPP
#define CRITICAL_STATE_HPP

#include "app/AppState.hpp"

#define RESTART_DELAY_SECONDS 10

class CriticalState : public AppState
{
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override
        {
            return "Critical";
        }

        AppStateType getType() const override
        {
            return CRITICAL;
        }
};

#endif
