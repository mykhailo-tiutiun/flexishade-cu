#include "app/ZigbeeInitState.hpp"

#include "app/AppContext.hpp"
#include "io/config/StateLed.hpp"

void ZigbeeInitState::onEnter() {

    context_->tryGetComponent<StateLed>().value()->amber();

    context_->transit_state(NORMAL);
}

void ZigbeeInitState::onExit() {

}

void ZigbeeInitState::toogleConfigMode() {

}


