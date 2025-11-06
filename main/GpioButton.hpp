#ifndef GPIO_BUTTON_HPP
#define GPIO_BUTTON_HPP

class GpioButton {
    private:
        static void buttonCallback(void *arg, void *usr_data) {
            GpioButton *self = (GpioButton *)usr_data;
            self->handleClick();
        }
    public:
        GpioButton(int gpio_num);
    protected:
        virtual void handleClick();
};

#endif