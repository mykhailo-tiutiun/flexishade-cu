#include "app/ConfigErrState.hpp"
#include "app/AppState.hpp"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "io/config/StateLed.hpp"
#include "app/AppContext.hpp"

static const char* TAG = "critical_state";

void ConfigErrState::onEnter()
{
    context_->tryGetComponent<StateLed>().value()->amber();
    ESP_LOGI(TAG, "config state error: retring in %d seconds ...", RETRY_DELAY_SECONDS);
    vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_SECONDS * 1000));

    context_->transit_state(CONFIG);
}

void ConfigErrState::onExit()
{
}

void ConfigErrState::toogleConfigMode()
{
    context_->transit_state(NORMAL);
}

