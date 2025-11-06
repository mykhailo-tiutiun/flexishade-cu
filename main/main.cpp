 
#include <stdio.h>
#include <inttypes.h>
#include "application/AppContext.hpp"
#include "freertos/projdefs.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ConfigButton.hpp"

extern "C" void app_main(void)
{
    printf("Hello world!\n");

    AppContext app;
    app.run_app();
    
    ConfigButton button = {20};
    button.setContext(&app);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
