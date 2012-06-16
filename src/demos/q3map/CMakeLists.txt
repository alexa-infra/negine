
set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
)

add_executable(q3map_demo
    q3mapdemo.cpp
)

add_dependencies(q3map_demo
    negine
    app
)

target_link_libraries(q3map_demo
    negine
    app
)
