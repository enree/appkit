project(config_test)

set(CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR})
generatePaths(appkit SRC)

gtest(${PROJECT_NAME}
    SOURCES ${SRC}
    GLOBBING "*.cpp" "*.h"
    RECURSIVE true
    QT Core
    LIBS appkit_config)

file(GLOB _files "*.json")
add_custom_target(test_config SOURCES ${_files})
