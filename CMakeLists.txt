cmake_minimum_required(VERSION 3.12)

project(appkit)

include(${CMAKE_SOURCE_DIR}/cmake/make.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/check/lint.cmake)
include(${CMAKE_SOURCE_DIR}/modules/gqtest/gqtest.cmake)

defineProjectIncludes(${CMAKE_CURRENT_BINARY_DIR})
include(${CMAKE_CURRENT_SOURCE_DIR}/cmakeModules/appkit.cmake)

set(IS_ROOT ON)

# Project main source
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
