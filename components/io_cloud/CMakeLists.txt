idf_component_register(
    SRCS
        "src/WifiSta.cpp"
        "src/MqttClient.cpp"
        "src/MqttSubscription.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        config

        esp_event
        esp_wifi
        mqtt
)
