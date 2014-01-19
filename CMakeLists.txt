cmake_minimum_required(VERSION 2.8)
project(negine)

include(cmake/internal_utils.cmake)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(NEGINE_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TOOLS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

if(NOT DEFINED SHARED)
	set(SHARED ON)
endif()

if(SHARED)
	set(NEGINE_SHARED_BUILD ON)
	set(NEGINE_STATIC_BUILD OFF)
else()
	set(NEGINE_SHARED_BUILD OFF)
	set(NEGINE_STATIC_BUILD ON)
endif()

if(THIRDPARTY_ONLY)
	add_subdirectory(${TOOLS_PATH})
elseif(DEFINED PREBUILD_THIRDPARTY_PATH)
	add_subdirectory(${NEGINE_SRC_PATH})
else()
	add_subdirectory(${TOOLS_PATH})
	add_subdirectory(${NEGINE_SRC_PATH})
endif()