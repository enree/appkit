project(plugins_test)

collectSources(SOURCES _src)
test(${PROJECT_NAME}
    SOURCES ${_src}
    QT Core Test
    LIBS coriolis_plugins coriolis_qt
    BOOST locale)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(PLUGIN_PATH ${CMAKE_CURRENT_BINARY_DIR}/plugins)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_PATH})

add_subdirectory(service)
add_subdirectory(nonservice)

configure_file(${TEMPLATES_PATH}/paths.h.in Paths.h @ONLY)
