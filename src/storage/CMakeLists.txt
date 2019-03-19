item(appkit_storage)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    RECURSIVE true
    BOOST filesystem
    QT Core
    DEPENDS appkit_core appkit_serialization)

lint(${PROJECT_NAME})

tests(storage)
