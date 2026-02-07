idf_component_register(
    SRCS
        "src/Zigbee.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        espressif__esp-zigbee-lib
        ieee802154
        esp_event

        app_rc
)
