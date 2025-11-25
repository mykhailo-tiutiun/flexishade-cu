#ifndef GPIO_BUTTON_HPP
#define GPIO_BUTTON_HPP

#include "freertos/FreeRTOS.h"

class GpioButton {
    private:
        TaskHandle_t button_task_;

        static void buttonISRCallback(void *arg, void *usr_data);
        static void buttonTaskHandler(void *arg);
    public:
        GpioButton(int gpio_num);
    protected:
        virtual void handleClick() {}
};

#endif