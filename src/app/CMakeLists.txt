item(appkit_app)

sharedLib(${PROJECT_NAME} 
    GLOBBING "*" 
    DEPENDS coriolis_core coriolis_qt
    QT Core
    BOOST filesystem system locale
    COMPONENT ${PROJECT_NAME})

lint(${PROJECT_NAME})

#tests(app)
