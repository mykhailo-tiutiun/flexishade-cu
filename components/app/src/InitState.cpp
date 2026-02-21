#include "app/InitState.hpp"

#include "app/AppContext.hpp"
#include "app/AppState.hpp"
#include "app/ConfigButton.hpp"

#include "config/ConfigController.hpp"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_pthread.h"

#include "nvs/Nvs.hpp"
#include "nvs/NvsRcRepository.hpp"
#include "nvs/NvsRelayRepository.hpp"
#include "nvs/NvsConfigRepository.hpp"
#include "rc/MqttRcController.hpp"
#include "rc/MqttRcPublisher.hpp"
#include "rc/RcService.hpp"
#include "wifi/WifiSta.hpp"
#include "wifi/WifiStaConfig.hpp"
#include "wifi/WifiAp.hpp"
#include "nvs_flash.h"

#include "io/config/StateLed.hpp"

#include "config/ConfigService.hpp"

#include "mqtt/MqttClient.hpp"
#include "dns/DnsServer.hpp"

#include "relay/Relay.hpp"
#include "relay/RelayService.hpp"
#include "relay/MqttRelayController.hpp"
#include "relay/MqttRelayPublisher.hpp"

#include "esp_coexist.h"

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

    auto nvs = new Nvs();
    context_->registerComponent(nvs);

    auto cfg_repository = new NvsConfigRepository(nvs);
    cfg_repository->registerConfig(std::make_unique<WifiStaConfig>());
    context_->registerComponent(cfg_repository);

    auto cfg_service = new ConfigService(cfg_repository);
    context_->registerComponent(cfg_service);

    auto mqtt = new MqttClient();
    context_->registerComponent(mqtt);

    auto wifi_sta = new WifiSta();
    context_->registerComponent(wifi_sta);

    auto nvs_relay_repository = new NvsRelayRepository(nvs);
    nvs_relay_repository->save(Relay(RelayId(1), 6));
    nvs_relay_repository->save(Relay(RelayId(2), 7));
    context_->registerComponent(nvs_relay_repository);

    MqttRelayPublisher* rpublisher = new MqttRelayPublisher(mqtt);
    context_->registerComponent(rpublisher);

    RelayService* rservice = new RelayService(nvs_relay_repository, rpublisher);
    context_->registerComponent(rservice);

    auto rcontroller = std::make_shared<MqttRelayController>(rservice);
    mqtt->addController(rcontroller);

    auto wifi_ap = new WifiAp();
    context_->registerComponent(wifi_ap);

    auto http_server = new HttpServer();
    context_->registerComponent(http_server);

    auto dns_server = new DnsServer();
    context_->registerComponent(dns_server);

    auto cfg_controller = new ConfigController(http_server, cfg_service);
    cfg_controller->registerHandlers();
    context_->registerComponent(cfg_controller);

    auto mqtt_rc_publisher = new MqttRcPublisher(mqtt);
    context_->registerComponent(mqtt_rc_publisher);

    auto nvs_rc_repo = new NvsRcRepository(nvs);
    context_->registerComponent(nvs_rc_repo);

    auto rc_service = new RcService(nvs_rc_repo, mqtt_rc_publisher, rservice);
    context_->registerComponent(rc_service);

    auto mqtt_rc_controller = std::make_shared<MqttRcController>(rc_service);

    mqtt->addController(mqtt_rc_controller);

    auto zigbee = new Zigbee(rc_service);
    zigbee->start();
    context_->registerComponent(zigbee);

    esp_coex_wifi_i154_enable();

    context_->tryGetComponent<StateLed>().value()->red();

    context_->transit_state(ZIGBEE_INIT);
}

void InitState::onExit() {

}

void InitState::toogleConfigMode() {

}
