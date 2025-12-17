#include "app/AppContext.hpp"

#include "app/AppState.hpp"
#include "app/InitState.hpp"
#include "app/ZigbeeInitState.hpp"
#include "app/NormalState.hpp"
#include "app/ConfigState.hpp"

#include "esp_log.h"
#include <cstdio>

static const char* TAG = "app context";

AppContext::AppContext() : state_(nullptr)
{
    addState(new InitState());
    addState(new ZigbeeInitState());
    addState(new NormalState());
    addState(new ConfigState());
};

void AppContext::run_app() {
    transit_state(INIT);
}

void AppContext::transit_state(AppStateType type) {
    auto state = getState(type);
    ESP_LOGI(TAG, "Transition to %s\n", state->getName());

    if (state_ != nullptr) {
        state_->onExit();
    }

    state_ = state;
    state_->setContext(this);
    state_->onEnter();

    ESP_LOGI(TAG, "Exited on enter %s\n", state->getName());
}

AppState* AppContext::getCurrentState() const {
    return state_;
}

AppState* AppContext::getState(AppStateType type)
{
    auto it = states_.find(type);
    if (it != states_.end()) {
        return it->second;
    }
    return nullptr;
}

void AppContext::addState(AppState* state)
{
    states_[state->getType()] = state;
}
