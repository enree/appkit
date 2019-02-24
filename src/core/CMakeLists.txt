item(appkit_core)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    QT Core
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true)

set_target_properties(${PROJECT_NAME} PROPERTIES
    "INTERFACE_INCLUDE_DIRECTORIES" "${APPKIT_PATH}/include/core")

lint(${PROJECT_NAME})

#tests(core)
