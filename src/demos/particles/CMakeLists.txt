
set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
)

add_executable(particles_demo
    particlesdemo.cpp
)

add_dependencies(particles_demo
    negine
    app
)

target_link_libraries(particles_demo
    negine
    app
)

