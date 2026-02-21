idf_component_register(
    SRCS
        "src/ConfigService.cpp"
        "src/ConfigController.cpp"
        "src/IndexPage.cpp"
    INCLUDE_DIRS
        "include"
    REQUIRES
        io_config
        app_http
        json
        nvs_flash
        app_misc

)
