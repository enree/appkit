item(appkit_app)

sharedLib(${PROJECT_NAME} 
    GLOBBING "*" 
    RECURSIVE true
    DEPENDS appkit_config appkit_log
    QT Core
    BOOST filesystem system locale log log_setup
    COMPONENT ${PROJECT_NAME})

lint(${PROJECT_NAME})

tests(app)
