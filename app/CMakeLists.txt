set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(APP_PATH ${SRC_PATH}/app)

add_source_group("app"          ${APP_PATH}         SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/freeglut/include
    ${TOOLS_PATH}/lua/include
)
add_executable(app ${SRC})

add_dependencies(tests
    freeglut
    base
    renderer
    lua
)

target_link_libraries(app 
    freeglut
    base
    renderer
    lua
)

add_definitions(-DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0 -DGLEW_STATIC)