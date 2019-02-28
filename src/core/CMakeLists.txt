item(appkit_core)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    QT Core
    COMPONENT ${PROJECT_NAME}
    RECURSIVE true)

lint(${PROJECT_NAME})

tests(core)
