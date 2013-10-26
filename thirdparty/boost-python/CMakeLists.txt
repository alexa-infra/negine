cmake_minimum_required(VERSION 2.8)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_LIB_PATH ${SRC_PATH}/libs/python/src)

macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)
    file(GLOB_RECURSE TEMP_SRC
        ${SOURCE_PATH}/*.h
        ${SOURCE_PATH}/*.hpp
        ${SOURCE_PATH}/*.cpp
        ${SOURCE_PATH}/*.cc
        ${SOURCE_PATH}/*.c
    )
    source_group(${FILTER_NAME} FILES ${TEMP_SRC})
    list(APPEND ${TARGET_LIST} ${TEMP_SRC})
endmacro(add_source_group) 

add_source_group("src"      ${SRC_LIB_PATH}           SRC)
add_source_group("include"  ${SRC_PATH}/boost      	  HDR)

if(MSVC)
	set(PYTHON_INCLUDE_DIR "d:/devtools/python27/include")
	set(PYTHON_LIBRARY "d:/devtools/python27/libs/python27.lib")
endif()

find_package(PythonLibs REQUIRED)
if (NOT PYTHONLIBS_FOUND)
	message(FATAL ERROR "Python is not found")
endif()

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -Wno-invalid-offsetof -std=gnu++11 -fno-exceptions -fno-rtti")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wattributes")

include_directories(
	${SRC_PATH}
	${SRC_LIB_PATH}
	${PYTHON_INCLUDE_DIRS}
)

add_library(boostpython SHARED
	${SRC}
)

target_link_libraries(boostpython
	${PYTHON_LIBRARIES}
)

add_definitions(-DBOOST_PYTHON_DYNAMIC_LIB
				-DBOOST_PYTHON_SOURCE
				-DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION)

if(WIN64 AND NOT MSVC)
	add_definitions(-DMS_WIN64)
endif()