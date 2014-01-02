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
    add_definitions(-DMS_NO_COREDLL -DPy_ENABLE_SHARED)
endif()

set(Python_ADDITIONAL_VERSIONS 3.3)
find_package(PythonLibs REQUIRED)
if (NOT PYTHONLIBS_FOUND)
	message(FATAL ERROR "Python is not found")
endif()

if(UNIX OR MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()

include_directories(
	${SRC_PATH}
	${SRC_LIB_PATH}
	${PYTHON_INCLUDE_DIRS}
)

if(BOOSTPYTHON_STATIC)
	add_library(boostpython-static STATIC ${SRC})

	target_link_libraries(boostpython-static
		debug ${PYTHON_LIBRARY_DEBUG}
		optimized ${PYTHON_LIBRARY_RELEASE}
	)

	set_target_properties(boostpython-static PROPERTIES
		OUTPUT_NAME "boostpython"
		)
endif()

if(BOOSTPYTHON_SHARED)
	add_library(boostpython SHARED ${SRC})

	target_link_libraries(boostpython
		debug ${PYTHON_LIBRARY_DEBUG}
		optimized ${PYTHON_LIBRARY_RELEASE}
	)

	set_target_properties(boostpython PROPERTIES
		COMPILE_FLAGS "-DBOOST_PYTHON_DYNAMIC_LIB"
		OUTPUT_NAME "boostpython"
		)
endif()

if (NOT ${PYTHON_LIBRARY_DEBUG} STREQUAL ${PYTHON_LIBRARY_RELEASE})
	add_definitions(-DBOOST_DEBUG_PYTHON)
endif()

add_definitions(-DBOOST_PYTHON_SOURCE
				-DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION)

set(SIZEOF_VOIDP ${CMAKE_SIZEOF_VOID_P})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ARCH_64 TRUE)
else()
	set(ARCH_64 FALSE)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)