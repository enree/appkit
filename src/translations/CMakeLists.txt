item(appkit_translations)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    DEPENDS appkit_core
    BOOST locale filesystem regex
    COMPONENT ${PROJECT_NAME})

lint(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    "INTERFACE_INCLUDE_DIRECTORIES" "${APPKIT_PATH}/include/translations")

