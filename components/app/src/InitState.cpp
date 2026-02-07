#include "app/InitState.hpp"

#include "app/AppContext.hpp"
#include "app/AppState.hpp"
#include "app/ConfigButton.hpp"

#include "config/ConfigController.hpp"
#include "config/MqttConfig.hpp"
#include "config/NvsConfigService.hpp"
#include "config/WifiConfig.hpp"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_pthread.h"
#include "esp_wifi.h"
#include "rc/RcRepository.hpp"
#include "rc/RcService.hpp"
#include "wifi/WifiSta.hpp"
#include "wifi/WifiAp.hpp"
#include "nvs_flash.h"

#include "io/config/StateLed.hpp"

#include "config/ConfigService.hpp"

#include "mqtt/MqttClient.hpp"
#include "dns/DnsServer.hpp"

#include "relay/Relay.hpp"
#include "relay/RelayService.hpp"
#include "relay/RelayDb.hpp"
#include "relay/MqttRelayController.hpp"
#include "relay/MqttRelayPublisher.hpp"
#include <memory>

#include "zigbee/Zigbee.hpp"

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

    auto cfg = esp_pthread_get_default_config();
    esp_pthread_set_cfg(&cfg);

    ConfigService* configs = new NvsConfigService();
    context_->registerComponent(configs);
    configs->registerConfig(std::make_shared<WifiConfig>());
    configs->registerConfig(std::make_shared<MqttConfig>());

    context_->registerComponent(new MqttClient());

    auto wifi_sta = new WifiSta();
    context_->registerComponent(wifi_sta);

    RelayDb* relaydb = new RelayDb();
    relaydb->save(Relay(RelayId(1), 6));
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

    auto wifi_ap = new WifiAp();
    context_->registerComponent(wifi_ap);

    auto http_server = new HttpServer();
    context_->registerComponent(http_server);

    auto dns_server = new DnsServer();
    context_->registerComponent(dns_server);

    auto conf_controller = new ConfigController(http_server, configs);
    conf_controller->registerHandlers();
    context_->registerComponent(conf_controller);

    auto rc_repo = new RcRepository();
    context_->registerComponent(rc_repo);

    auto rc_service = new RcService(rc_repo, rservice);
    context_->registerComponent(rc_service);

    auto zigbee = new Zigbee(rc_service);
    zigbee->start();
    context_->registerComponent(zigbee);

    context_->tryGetComponent<StateLed>().value()->red();

    context_->transit_state(ZIGBEE_INIT);
}

void InitState::onExit() {

}

void InitState::toogleConfigMode() {

}
