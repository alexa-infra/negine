cmake_minimum_required(VERSION 2.8)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB FOUNDATION_SRC ${SRC_PATH}/*.cpp)
file(GLOB FOUNDATION_HDR ${SRC_PATH}/*.h)
source_group("Headers" FILES ${FOUNDATION_HDR})
source_group("Sources" FILES ${FOUNDATION_SRC}) 

include_directories(
	${SRC_PATH}
)

add_library(bitsquid-foundation STATIC ${FOUNDATION_SRC} ${FOUNDATION_HDR})
