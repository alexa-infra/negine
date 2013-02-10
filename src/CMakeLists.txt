set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

add_source_group("base"         ${SRC_PATH}/base        BASE_SRC)
add_source_group("math"         ${SRC_PATH}/math        MATH_SRC)
add_source_group("render"       ${SRC_PATH}/render      RENDER_SRC)
add_source_group("game"         ${SRC_PATH}/game        GAME_SRC)
add_source_group("glew"         ${TOOLS_PATH}/glew/src  GLEW_SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/lua/include
)

add_library(negine
    ${MATH_SRC}
    ${BASE_SRC}
    ${RENDER_SRC}
    ${GAME_SRC}
    ${GLEW_SRC}
)

if(WIN32)
    target_link_libraries(negine OpenGL32)
endif(WIN32)
if(UNIX AND NOT APPLE)
    target_link_libraries(negine GL)
endif()
if(WIN32)
	add_definitions(-DGLEW_STATIC)
endif()
