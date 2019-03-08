# @file
# @brief Adding appkit components
#

if (DEFINED APPKIT_ROOT)
    return()
endif()
set(APPKIT_ROOT ${CMAKE_CURRENT_LIST_DIR})

include(${APPKIT_ROOT}/cmakeModules/appkit.cmake)

add_subdirectory(${APPKIT_ROOT})
