project(app_test)

gtest(${PROJECT_NAME}
    GLOBBING "*" 
    RECURSIVE false
    QT Core
    LIBS coriolis_app coriolis_qt
    BOOST filesystem regex system thread)

set(PLUGIN_PATH ${CMAKE_CURRENT_BINARY_DIR}/plugins)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_PATH})

include_directories(.)
add_subdirectory(aux)

configure_file(${TEMPLATES_PATH}/paths.h.in Paths.h @ONLY)
