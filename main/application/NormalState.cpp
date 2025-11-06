#include "NormalState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"

void NormalState::onEnter() {
    
    context_->getStateLed()->green();
}

void NormalState::onExit() {

}

void NormalState::toogleConfigMode() {
    AppState* next_state = context_->getAppStateFactory()->config();
    context_->transit_state(next_state);
}

const char* NormalState::getName() const {
    return "Normal";
}