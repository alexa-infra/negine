cmake_minimum_required(VERSION 2.8)

set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB FOUNDATION_SRC ${SRC_PATH}/src/*.cpp)
file(GLOB FOUNDATION_HDR ${SRC_PATH}/foundation/*.h)
source_group("Headers" FILES ${FOUNDATION_HDR})
source_group("Sources" FILES ${FOUNDATION_SRC}) 

include_directories(
	${SRC_PATH}
)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif()

add_library(bitsquid-foundation STATIC ${FOUNDATION_SRC} ${FOUNDATION_HDR})

install(TARGETS bitsquid-foundation RUNTIME DESTINATION bin
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib)

install(FILES ${FOUNDATION_HDR} DESTINATION include/foundation)