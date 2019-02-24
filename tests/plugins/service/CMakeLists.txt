project(service)

collectSources(SOURCES _PLUGIN_SOURCES)
qtPlugin(${PROJECT_NAME} SOURCES ${_PLUGIN_SOURCES} QT Core NOINSTALL true)
