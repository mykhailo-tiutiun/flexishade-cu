#include "app/NormalErrState.hpp"
#include "app/AppState.hpp"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "io/config/StateLed.hpp"
#include "app/AppContext.hpp"

static const char* TAG = "critical_state";

void NormalErrState::onEnter()
{
    context_->tryGetComponent<StateLed>().value()->amber();
    ESP_LOGI(TAG, "normal state error: retring in %d seconds ...", RETRY_DELAY_SECONDS);
    context_->transit_state(NORMAL);
    vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_SECONDS * 1000));
}

void NormalErrState::onExit()
{
}

void NormalErrState::toogleConfigMode()
{
    context_->transit_state(CONFIG);
}

