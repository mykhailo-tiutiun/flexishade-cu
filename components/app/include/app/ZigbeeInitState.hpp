#ifndef ZIGBEE_INIT_STATE_HPP
#define ZIGBEE_INIT_STATE_HPP

#include "app/AppState.hpp"

class ZigbeeInitState: public AppState {
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override
        {
            return "ZigbeeInit";
        }

        AppStateType getType() const override
        {
            return ZIGBEE_INIT;
        }
};

#endif
