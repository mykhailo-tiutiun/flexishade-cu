idf_component_register(
    SRCS
        "src/WifiSta.cpp"
        "src/WifiStaConfig.cpp"
        "src/WifiAp.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        esp_event
        esp_wifi
        nvs_flash
        json

        app_config
)
