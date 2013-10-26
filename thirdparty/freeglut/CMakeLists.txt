cmake_minimum_required(VERSION 2.8)
project(freeglut)

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

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_source_group("src"      ${SRC_PATH}/src          SRC)
add_source_group("include"  ${SRC_PATH}/include/GL   SRC)

include_directories(${SRC_PATH}/include ${SRC_PATH}/src)

if(FREEGLUT_STATIC)
	add_library(freeglut-static STATIC ${SRC})
	set_target_properties(freeglut-static PROPERTIES
		COMPILE_FLAGS "-DFREEGLUT_STATIC"
		OUTPUT_NAME "freeglut")
endif()

if(FREEGLUT_SHARED)
	add_library(freeglut SHARED ${SRC})
	set_target_properties(freeglut PROPERTIES
		COMPILE_FLAGS "-DFREEGLUT_EXPORTS"
		OUTPUT_NAME "freeglut")
	target_link_libraries(freeglut glu32.lib opengl32.lib winmm.lib gdi32.lib user32.lib)
endif()

add_definitions(-DFREEGLUT_LIB_PRAGMAS=0)

if(UNIX)
  add_definitions(-DHAVE_SYS_TYPES_H -DHAVE_UNISTD_H -DTIME_WITH_SYS_TIME -DHAVE_FCNTL_H -DHAVE_VFPRINTF=1)
endif(UNIX)
