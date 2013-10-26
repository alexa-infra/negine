cmake_minimum_required(VERSION 2.8)
project(negine)

include(cmake/internal_utils.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TOOLS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

add_subdirectory(${SRC_PATH})
add_subdirectory(${TOOLS_PATH})
