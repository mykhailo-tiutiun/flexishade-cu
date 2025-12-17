
#include "app/AppContext.hpp"
#include "freertos/idf_additions.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char* TAG = "main";

extern "C" void app_main(void)
{
    AppContext* app = new AppContext();
    app->run_app();

    vTaskSuspend(NULL);
}
