
set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
)

add_executable(md5model_demo
    md5modeldemo.cpp
)

add_dependencies(md5model_demo
    negine
    app
)

target_link_libraries(md5model_demo
    negine
    app
)
