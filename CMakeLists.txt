cmake_minimum_required(VERSION 2.8)
project(negine)

set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)
set(APP_PATH ${SRC_PATH}/app)

set(CMAKE_MODULE_PATH ${SRC_PATH}/CMake ${CMAKE_MODULE_PATH})
include(${SRC_PATH}/CMake/CMakeHelpers.txt)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  add_definitions(-D_DEBUG)
endif(UNIX)
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WX")
endif(WIN32)
set(CMAKE_DEBUG_POSTFIX "_d")

add_subdirectory(${TOOLS_PATH}/freeglut ${PROJECT_BINARY_DIR}/freeglut)
add_subdirectory(${TOOLS_PATH}/glew ${PROJECT_BINARY_DIR}/glew)

add_subdirectory(${SRC_PATH}/base ${PROJECT_BINARY_DIR}/base)
add_subdirectory(${SRC_PATH}/renderer ${PROJECT_BINARY_DIR}/renderer)

add_subdirectory(${TESTS_PATH} ${PROJECT_BINARY_DIR}/tests)
add_subdirectory(${APP_PATH} ${PROJECT_BINARY_DIR}/app)

