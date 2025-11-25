 
#include <stdio.h>
#include <inttypes.h>
#include "application/AppContext.hpp"
#include "freertos/projdefs.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ConfigButton.hpp"
#include "esp_log.h"

extern "C" void app_main(void)
{
    printf("Hello world!\n");

    esp_log_level_set("wifi", ESP_LOG_DEBUG);

    AppContext app;
    app.run_app();
    
    ConfigButton button = {20};
    button.setContext(&app);

    vTaskSuspend(NULL);
}
