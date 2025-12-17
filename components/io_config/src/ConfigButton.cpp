#include "ConfigButton.hpp"
#include "io/config/GpioButton.hpp"
#include "application/AppContext.hpp"
#include "application/AppState.hpp"

ConfigButton::ConfigButton(int gpio_num) : GpioButton(gpio_num), context_(nullptr) {}

void ConfigButton::handleClick(){
    if (context_ != nullptr){
        context_->getCurrentState()->toogleConfigMode();
    }
}

void ConfigButton::setContext(AppContext *context){
    context_ = context;
}
