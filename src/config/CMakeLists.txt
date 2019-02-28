item(appkit_config)

sharedLib(${PROJECT_NAME}
    GLOBBING "*"
    DEPENDS appkit_core appkit_translations
    BOOST program_options 
    COMPONENT ${PROJECT_NAME})

lint(${PROJECT_NAME})

tests(config)
