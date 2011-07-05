set(SRC_PATH ${negine_SOURCE_DIR})
set(BASE_PATH ${SRC_PATH}/base)
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(APP_PATH ${SRC_PATH}/app)
set(RENDERER_PATH ${SRC_PATH}/renderer)

set(GLEW_PATH ${TOOLS_PATH}/GL)

add_source_group("base"         ${BASE_PATH}        SRC)
add_source_group("base/math"    ${BASE_PATH}/math   SRC)
add_source_group("app"          ${APP_PATH}         SRC)
add_source_group("renderer"     ${RENDERER_PATH}    SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/freeglut/include
)
add_executable(app ${SRC})

add_dependencies(tests
    freeglut
    glew
)

target_link_libraries(app 
    freeglut
    glew
)

if(WIN32)
  target_link_libraries(app OpenGL32 winmm user32)
endif(WIN32)
if(UNIX)
  target_link_libraries(app pthread GL)
endif(UNIX)

set_target_properties(app PROPERTIES
    COMPILE_FLAGS "-DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0"
)