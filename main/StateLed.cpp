#include "StateLed.hpp"
#include "led_strip.h"
#include "led_strip_rmt.h"
#include "led_strip_types.h"
#include "driver/gpio.h"

StateLed::StateLed() : led_strip(nullptr) {
    led_strip_config_t strip_config = {
        .strip_gpio_num = 8,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812,
    };
    
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
        .flags = {
            .with_dma = false,
        }
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip);
}

void StateLed::set_color(uint32_t r, uint32_t g, uint32_t b) {
    led_strip_set_pixel(led_strip, 0, g, r, b);
    led_strip_refresh(led_strip);
}

void StateLed::blue() {
    set_color(0, 0, 16);
}

void StateLed::red() {
    set_color(16, 0, 0);
}

void StateLed::green() {
    set_color(0, 16, 0);
}

void StateLed::amber() {
    set_color(16, 4, 0);
}