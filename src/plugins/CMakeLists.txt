item(appkit_plugins)

sharedLib(${PROJECT_NAME} GLOBBING "*"
    DEPENDS coriolis_core coriolis_qt coriolis_locale
    QT Core
    COMPONENT ${PROJECT_NAME})

lint(${PROJECT_NAME})

tests(plugins)
