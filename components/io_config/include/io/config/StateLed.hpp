# ifndef STATE_LED_HPP
# define STATE_LED_HPP

#include "led_strip.h"

#define STATE_LED_GPIO 8

class StateLed {
    private:
        led_strip_handle_t led_strip;

        void set_color(uint32_t r, uint32_t g, uint32_t b);
    public:
        StateLed();
        void blue();
        void red();
        void green();
        void amber();
};

# endif