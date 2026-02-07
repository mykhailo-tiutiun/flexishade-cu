idf_component_register(
    SRCS
        "src/Relay.cpp"
        "src/RelayDb.cpp"
        "src/RelayService.cpp"
        "src/MqttRelayController.cpp"
        "src/MqttRelayPublisher.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        esp_event
        json
        driver

        app_mqtt
)
