project(app_test)

set(AUX_PATH ${CMAKE_CURRENT_BINARY_DIR}/aux_app)
add_definitions(-DAUX_PATH="${AUX_PATH}")

gtest(${PROJECT_NAME}
    GLOBBING "*" 
    RECURSIVE false
    QT Core
    LIBS appkit_app appkit_core
    BOOST filesystem regex system thread)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${AUX_PATH})

include_directories(.)
add_subdirectory(aux)
