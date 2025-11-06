#ifndef CONFIG_BUTTON_HPP
#define CONFIG_BUTTON_HPP

#include "GpioButton.hpp"

class AppContext;

class ConfigButton: public GpioButton {
    private:
        AppContext *context_;
    protected:
        void handleClick() override;
    public:
        ConfigButton(int gpio_num);
        void setContext(AppContext *context);
};

#endif