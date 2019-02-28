project(appkit)

# Add translation
generateTranslationMacros(APPKIT
    ${PROJECT_GENERATED_INCLUDES}/${PROJECT_NAME})

#add_subdirectory(app)
add_subdirectory(config)
add_subdirectory(core)
add_subdirectory(log)
add_subdirectory(plugins)
add_subdirectory(translations)

#compileTranslations()
