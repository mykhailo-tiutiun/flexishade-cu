#ifndef ZIGBEE_INIT_STATE_HPP
#define ZIGBEE_INIT_STATE_HPP

#include "AppState.hpp"

class ZigbeeInitState: public AppState {
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override;
};

#endif