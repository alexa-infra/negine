
set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
)

add_executable(scene_demo
    scenedemo.cpp
)

add_dependencies(scene_demo
    negine
    app
)

target_link_libraries(scene_demo
    negine
    app
)
