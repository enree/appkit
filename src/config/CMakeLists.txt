item(appkit_config)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    DEPENDS appkit_core appkit_translations
    BOOST program_options 
    COMPONENT ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    "INTERFACE_INCLUDE_DIRECTORIES" "${APPKIT_PATH}/include/config")

lint(${PROJECT_NAME})

tests(config)
