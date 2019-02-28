item(appkit_translations)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    DEPENDS appkit_core
    BOOST locale filesystem regex
    COMPONENT ${PROJECT_NAME})

lint(${PROJECT_NAME})


