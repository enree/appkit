item(appkit_plugins)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    DEPENDS appkit_core
    QT Core
    COMPONENT ${PROJECT_NAME})

lint(${PROJECT_NAME})

tests(plugins)
