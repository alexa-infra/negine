cmake_minimum_required(VERSION 2.8)
project(negine)

set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)

option(USE_SDL "SDL" ON)
option(USE_GLUT "Freeglut" OFF)

if (USE_SDL AND USE_GLUT)
  message(FATAL_ERROR "use only one sdl or glut option")
endif()
if (NOT USE_SDL AND NOT USE_GLUT)
  message(FATAL_ERROR "use one engine: sdl or glut")
endif()

macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)
    file(GLOB TEMP_SRC
        ${SOURCE_PATH}/*.h
        ${SOURCE_PATH}/*.cpp
        ${SOURCE_PATH}/*.cc
        ${SOURCE_PATH}/*.c
    )
    source_group(${FILTER_NAME} FILES ${TEMP_SRC})
    list(APPEND ${TARGET_LIST} ${TEMP_SRC})
endmacro(add_source_group)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -std=c++0x")
#  add_definitions(-D_DEBUG)
endif(UNIX)
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WX")
endif(WIN32)
set(CMAKE_DEBUG_POSTFIX "_d")

if(USE_GLUT)
  add_subdirectory(${TOOLS_PATH}/freeglut ${PROJECT_BINARY_DIR}/freeglut)
endif(USE_GLUT)
add_subdirectory(${TOOLS_PATH}/glew ${PROJECT_BINARY_DIR}/glew)
add_subdirectory(${TOOLS_PATH}/lua ${PROJECT_BINARY_DIR}/lua)
add_subdirectory(${TOOLS_PATH}/jsoncpp ${PROJECT_BINARY_DIR}/jsoncpp)

add_subdirectory(${SRC_PATH} ${PROJECT_BINARY_DIR}/negine)
add_subdirectory(${SRC_PATH}/app ${PROJECT_BINARY_DIR}/app)
add_subdirectory(${TESTS_PATH} ${PROJECT_BINARY_DIR}/tests)
