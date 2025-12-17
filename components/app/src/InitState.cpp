#include "app/InitState.hpp"

#include "app/AppContext.hpp"
#include "app/AppState.hpp"
#include "app/ConfigButton.hpp"

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "io/config/StateLed.hpp"
#include "io/cloud/MqttClient.hpp"

#include "relay/Relay.hpp"
#include "relay/RelayService.hpp"
#include "relay/RelayDb.hpp"
#include "relay/MqttRelayController.hpp"
#include "relay/MqttRelayPublisher.hpp"


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

    context_->registerComponent(new MqttClient());

    RelayDb* relaydb = new RelayDb();
    relaydb->save(Relay(LocalId(1), 6));
    context_->registerComponent(relaydb);

    auto rpublisher_mqtt = context_->tryGetComponent<MqttClient>().value();
    MqttRelayPublisher* rpublisher = new MqttRelayPublisher(rpublisher_mqtt);
    context_->registerComponent(rpublisher);

    RelayDb* rservice_db = context_->tryGetComponent<RelayDb>().value();
    MqttRelayPublisher* rservice_publisher = context_->tryGetComponent<MqttRelayPublisher>().value();
    RelayService* rservice = new RelayService(rservice_db, rservice_publisher);
    context_->registerComponent(rservice);

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
