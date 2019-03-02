project(appkit)

set (TRANSLATION_DOMAIN "appkit")
# Add translation
generateTranslationMacros(APPKIT
    ${PROJECT_GENERATED_INCLUDES}/${PROJECT_NAME} TR_DOMAIN ${TRANSLATION_DOMAIN})

#add_subdirectory(app)
add_subdirectory(config)
add_subdirectory(core)
add_subdirectory(log)
add_subdirectory(plugins)
add_subdirectory(translations)

#compileTranslations()
