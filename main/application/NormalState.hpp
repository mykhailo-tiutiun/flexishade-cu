#ifndef NORMAL_STATE_HPP
#define NORMAL_STATE_HPP

#include "AppState.hpp"

class NormalState: public AppState {
    public:
        void onEnter() override;
        void onExit() override;

        void toogleConfigMode() override;

        const char* getName() const override;
};

#endif