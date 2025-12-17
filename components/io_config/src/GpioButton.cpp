#include "io/config/GpioButton.hpp"
#include "button_gpio.h"
#include "freertos/idf_additions.h"
#include "iot_button.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"


static const char *TAG = "GpioButton";

void GpioButton::buttonISRCallback(void *arg, void *usr_data){
    GpioButton* me = (GpioButton *)usr_data;
    xTaskNotifyFromISR(me->button_task_, 1, eSetBits, NULL);
}

void GpioButton::buttonTaskHandler(void *arg){
    GpioButton* me = (GpioButton *)arg;
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        me->handleClick();
    }
}

GpioButton::GpioButton(int gpio_num) : button_task_(nullptr) {
    esp_err_t ret = xTaskCreate(
        buttonTaskHandler,
        "ButtonTask",
        4096,
        (void *)this,
        3,
        &button_task_
    );

    if (ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create button task");
        return;
    }

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
    iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &gpio_btn);
    if(NULL == gpio_btn) {
        ESP_LOGE(TAG, "Button create failed");
    } else {
        iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, &args, buttonISRCallback, (void *) this);
    }
}
