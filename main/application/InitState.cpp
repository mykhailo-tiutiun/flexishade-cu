#include "InitState.hpp"
#include "../StateLed.hpp"
#include "AppContext.hpp"
#include "AppState.hpp"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "../ConfigButton.hpp"
#include "../domain/model/Relay.hpp"
#include "../domain/service/RelayService.hpp"
#include "../db/RelayDb.hpp"
#include "../mqtt/MqttClient.hpp"
#include "../mqtt/MqttRelayController.hpp"


void InitState::onEnter() {
    StateLed *led = new StateLed();
    context_->registerComponent(led);

    ConfigButton* button = new ConfigButton(20);
    button->setContext(context_);
    context_->registerComponent(button);

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    RelayDb* relaydb = new RelayDb();
    relaydb->save(Relay(LocalId(1), 6));
    context_->registerComponent(relaydb);

    RelayDb* rservice_db = context_->tryGetComponent<RelayDb>().value();
    RelayService* rservice = new RelayService(rservice_db);
    context_->registerComponent(rservice);

    context_->registerComponent(new MqttClient());

    auto rcontroller_mqtt = context_->tryGetComponent<MqttClient>();
    auto rcontroller_service = context_->tryGetComponent<RelayService>();
    MqttRelayController* rcontroller = new MqttRelayController(rcontroller_mqtt.value(), rcontroller_service.value());
    rcontroller->subscribeAll();
    context_->registerComponent(rcontroller);

    context_->tryGetComponent<StateLed>().value()->red();

    context_->transit_state(ZIGBEE_INIT);
}

void InitState::onExit() {

}

void InitState::toogleConfigMode() {

}
