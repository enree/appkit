item(appkit_serialization)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    QT Core
    DEPENDS appkit_core)

lint(${PROJECT_NAME})
