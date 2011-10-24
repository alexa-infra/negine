set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)

add_source_group("renderer"         ${SRC_PATH}/renderer        SRC)

include_directories(
    ${TOOLS_PATH}
    ${TOOLS_PATH}/glew/include
    ${SRC_PATH}
)
add_library(renderer ${SRC})

add_dependencies(renderer glew base)

target_link_libraries(renderer glew base)

if(WIN32)
  target_link_libraries(renderer OpenGL32 winmm user32)
endif(WIN32)
if(UNIX)
  target_link_libraries(renderer GL)
endif(UNIX)

add_definitions(-DGLEW_STATIC)