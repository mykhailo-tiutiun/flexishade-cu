#include "ConfigState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"

void ConfigState::onEnter() {
    
    context_->getStateLed()->blue();

}

void ConfigState::onExit() {

}

void ConfigState::toogleConfigMode() {
    AppState* next_state = context_->getAppStateFactory()->normal();
    context_->transit_state(next_state);
}

const char* ConfigState::getName() const {
    return "Config";
}