#include "InitState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"
#include "AppStateFactory.hpp"

void InitState::onEnter() {
    StateLed *led = new StateLed();
    context_->setStateLed(led);
    
    led->red();

    AppState* next_state = context_->getAppStateFactory()->zigbeeInit();
    context_->transit_state(next_state);
}

void InitState::onExit() {

}

void InitState::toogleConfigMode() {

}

const char* InitState::getName() const {
    return "Init";
}