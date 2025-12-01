
#include <cstdio>
#include <stdio.h>
#include "application/AppContext.hpp"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ConfigButton.hpp"
#include "esp_log.h"

static const char* TAG = "main";

extern "C" void app_main(void)
{
    AppContext* app = new AppContext();
    app->run_app();

    vTaskSuspend(NULL);
}
