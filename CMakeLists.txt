cmake_minimum_required(VERSION 2.8)
project(negine)

set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)
set(APP_PATH ${SRC_PATH}/app)

option(USE_SDL "SDL" ON)
option(USE_GLUT "Freeglut" OFF)

if (USE_SDL AND USE_GLUT)
  message(FATAL_ERROR "use only one sdl or glut option")
endif()
if (NOT USE_SDL AND NOT USE_GLUT)
  message(FATAL_ERROR "use one engine: sdl or glut")
endif()

set(CMAKE_MODULE_PATH ${SRC_PATH}/CMake ${CMAKE_MODULE_PATH})
include(${SRC_PATH}/CMake/CMakeHelpers.txt)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -std=c++0x")
  add_definitions(-D_DEBUG)
endif(UNIX)
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WX")
endif(WIN32)
set(CMAKE_DEBUG_POSTFIX "_d")

add_subdirectory(${TOOLS_PATH}/freeglut ${PROJECT_BINARY_DIR}/freeglut)
add_subdirectory(${TOOLS_PATH}/glew ${PROJECT_BINARY_DIR}/glew)
add_subdirectory(${TOOLS_PATH}/lua ${PROJECT_BINARY_DIR}/lua)

add_subdirectory(${SRC_PATH}/base ${PROJECT_BINARY_DIR}/base)
add_subdirectory(${SRC_PATH}/renderer ${PROJECT_BINARY_DIR}/renderer)

add_subdirectory(${TESTS_PATH} ${PROJECT_BINARY_DIR}/tests)
add_subdirectory(${APP_PATH} ${PROJECT_BINARY_DIR}/app)

