
#include "app/AppContext.hpp"
#include "freertos/idf_additions.h"
#include "freertos/task.h"

static const char* TAG = "main";

extern "C" void app_main(void)
{
    auto app = new AppContext();
    app->run_app();

    vTaskSuspend(nullptr);
}
