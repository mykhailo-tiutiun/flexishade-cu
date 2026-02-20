idf_component_register(
    SRCS
        "src/Nvs.cpp"
        "src/NvsRcRepository.cpp"
        "src/NvsRelayRepository.cpp"
        "src/NvsConfigRepository.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        esp_event
        nvs_flash

        app_rc
        app_relay
        app_misc
)
