#include "app/CriticalState.hpp"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/idf_additions.h"
#include "io/config/StateLed.hpp"
#include "app/AppContext.hpp"

static const char* TAG = "critical_state";

void CriticalState::onEnter()
{
    context_->tryGetComponent<StateLed>().value()->red();
    ESP_LOGI(TAG, "critical error: restarting in %d seconds ...", RESTART_DELAY_SECONDS);
    vTaskDelay(pdMS_TO_TICKS(RESTART_DELAY_SECONDS * 1000));
    esp_restart();
}

void CriticalState::onExit()
{
}

void CriticalState::toogleConfigMode()
{}

