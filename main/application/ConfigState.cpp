#include "ConfigState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"
#include "../wifi/SoftAp.hpp"

ConfigState::ConfigState() : soft_ap_(nullptr) {}

void ConfigState::onEnter() {

    soft_ap_ = new SoftAp();
    context_->getStateLed()->blue();

}

void ConfigState::onExit() {
    delete soft_ap_;
    soft_ap_ = nullptr;
}

void ConfigState::toogleConfigMode() {
    AppState* next_state = context_->getAppStateFactory()->normal();
    context_->transit_state(next_state);
}

const char* ConfigState::getName() const {
    return "Config";
}