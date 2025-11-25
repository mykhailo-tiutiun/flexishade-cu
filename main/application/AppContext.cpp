#include "AppContext.hpp"
#include "AppState.hpp"
#include <cstdio>
#include "InitState.hpp"

AppContext::AppContext() : state_(nullptr) {};

void AppContext::run_app() {
    transit_state(new InitState());
}

void AppContext::transit_state(AppState* state) {
    if (state_ != nullptr) {
        state_->onExit();
        delete state_;
    }

    printf("Context: Transition to %s\n", state->getName());

    state_ = state;
    state_->setContext(this);
    state_->onEnter();
}

AppState* AppContext::getState() const {
    return state_;
}

void AppContext::setStateLed(StateLed *led) {
    state_led_ = led;
}
StateLed* AppContext::getStateLed() const {
    return state_led_;
}

void AppContext::setAppStateFactory(AppStateFactory *factory) {
    state_factory_ = factory;
}
AppStateFactory* AppContext::getAppStateFactory() {
    return state_factory_;
}