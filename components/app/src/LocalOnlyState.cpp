#include "app/LocalOnlyState.hpp"
#include "app/AppState.hpp"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "io/config/StateLed.hpp"
#include "app/AppContext.hpp"
#include <chrono>
#include <thread>
#include "esp_pthread.h"

#define RETRY_SLEEP_DELTA_SECS 1

static const char* TAG = "local_only_state";

void LocalOnlyState::onEnter()
{
    context_->tryGetComponent<StateLed>().value()->amber();

    ESP_LOGI(TAG, "Proceding in local only: retring Normal in %d seconds ...", RETRY_DELAY_SECONDS);

    retryAfter();
}

void LocalOnlyState::onExit()
{
    if (retry_thread_) {
        (*retry_thread_).join();
        retry_thread_ = {};
    }
}

void LocalOnlyState::toogleConfigMode()
{
    cancelRetry();
    context_->transit_state(CONFIG);
}

void LocalOnlyState::retryTask() const
{
    for (int i = 0; i < RETRY_DELAY_SECONDS && !is_retry_canceled_.load(); i += RETRY_SLEEP_DELTA_SECS) {
        std::this_thread::sleep_for(std::chrono::seconds(RETRY_SLEEP_DELTA_SECS));
    }

    if (!is_retry_canceled_.load()) {
        context_->transit_state(NORMAL);
    }
}

void LocalOnlyState::retryAfter()
{
    is_retry_canceled_ = false;

    retry_thread_ = std::thread(&LocalOnlyState::retryTask, this);
}

void LocalOnlyState::cancelRetry()
{
    is_retry_canceled_ = true;
}
