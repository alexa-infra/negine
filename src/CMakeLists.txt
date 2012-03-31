set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

add_source_group("base"         ${SRC_PATH}/base        BASE_SRC)
add_source_group("base/math"    ${SRC_PATH}/base/math   MATH_SRC)
add_source_group("renderer"     ${SRC_PATH}/renderer    RENDERER_SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/lua/include
)

add_definitions(-DGLEW_STATIC)

add_library(negine
    ${MATH_SRC}
    ${BASE_SRC}
    ${RENDERER_SRC}
)
add_dependencies(negine
    lua
    jsoncpp
    glew
)
target_link_libraries(negine 
    lua
    jsoncpp
    glew
)
if(WIN32)
    target_link_libraries(negine OpenGL32)
endif(WIN32)
if(UNIX)
    target_link_libraries(negine GL)
endif(UNIX)