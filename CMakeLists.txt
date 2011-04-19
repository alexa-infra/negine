cmake_minimum_required(VERSION 2.8)
project(nengine)

set(SRC_PATH ${nengine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)
set(APP_PATH ${SRC_PATH}/app)

set(CMAKE_MODULE_PATH ${SRC_PATH}/CMake ${CMAKE_MODULE_PATH})
include(${SRC_PATH}/CMake/CMakeHelpers.txt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG -Werror")
set(CMAKE_DEBUG_POSTFIX "_d")

add_subdirectory(${TESTS_PATH} ${PROJECT_BINARY_DIR}/tests)
add_subdirectory(${APP_PATH} ${PROJECT_BINARY_DIR}/app)
