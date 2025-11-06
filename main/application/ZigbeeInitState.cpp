#include "ZigbeeInitState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"

void ZigbeeInitState::onEnter() {
    
    context_->getStateLed()->amber();

    AppState* next_state = context_->getAppStateFactory()->normal();
    context_->transit_state(next_state);
}

void ZigbeeInitState::onExit() {

}

void ZigbeeInitState::toogleConfigMode() {

}

const char* ZigbeeInitState::getName() const {
    return "ZigbeeInit";
}