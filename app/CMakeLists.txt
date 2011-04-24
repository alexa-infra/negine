set(SRC_PATH ${nengine_SOURCE_DIR})
set(BASE_PATH ${SRC_PATH}/base)
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(APP_PATH ${SRC_PATH}/app)
set(RENDERER_PATH ${SRC_PATH}/renderer)

set(GLEW_PATH ${TOOLS_PATH}/GL)

add_source_group("base"         ${BASE_PATH}        SRC)
add_source_group("base/math"    ${BASE_PATH}/math   SRC)
add_source_group("glew"         ${GLEW_PATH}        SRC)
add_source_group("app"          ${APP_PATH}         SRC)
add_source_group("renderer"     ${RENDERER_PATH}    SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
)
add_executable(app ${SRC})

target_link_libraries(app pthread glut GL)
