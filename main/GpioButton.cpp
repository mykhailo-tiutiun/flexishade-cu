#include "GpioButton.hpp"
#include "button_gpio.h"
#include "iot_button.h"
#include "esp_log.h"

static const char *TAG = "GpioButton";

GpioButton::GpioButton(int gpio_num) {
    const button_config_t btn_cfg = {0, 0};
    const button_gpio_config_t btn_gpio_cfg = {
        .gpio_num = gpio_num,
        .active_level = 0,
        .enable_power_save = false,
        .disable_pull = false,
    };

    button_event_args_t args = {
        .long_press {
            .press_time = 2000,
        },
    };

    button_handle_t gpio_btn = NULL;
    esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &gpio_btn);
    if(NULL == gpio_btn) {
        ESP_LOGE(TAG, "Button create failed");
    } else {
        iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, &args, buttonCallback, (void *) this);
    }
}

void GpioButton::handleClick() {}