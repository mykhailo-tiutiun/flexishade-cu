idf_component_register(
    SRCS
        "src/Nvs.cpp"
        "src/NvsRcRepository.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        esp_event
        nvs_flash

        app_rc
        app_misc
)
