#include "ZigbeeInitState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"

void ZigbeeInitState::onEnter() {

    context_->tryGetComponent<StateLed>().value()->amber();

    context_->transit_state(NORMAL);
}

void ZigbeeInitState::onExit() {

}

void ZigbeeInitState::toogleConfigMode() {

}


