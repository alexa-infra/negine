set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(APP_PATH ${SRC_PATH}/app)

set(SRC
    ${APP_PATH}/demo.h
    ${APP_PATH}/demo.cpp
    ${APP_PATH}/glutapp.h
    ${APP_PATH}/glutapp.cpp
    ${APP_PATH}/luavm.h
    ${APP_PATH}/main.cpp
)
source_group("app" FILES ${SRC})

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/freeglut/include
    ${TOOLS_PATH}/lua/include
)
add_executable(app ${SRC})

add_dependencies(app
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

find_package(SDL 1.3) 
if(NOT SDL_FOUND)
    message("SDL is not found")
else()
    set(SDLSRC
        ${APP_PATH}/sdldemo.cpp
        ${APP_PATH}/sdlapp.cpp
    )
    source_group("sdlapp" FILES ${SDLSRC})
    include_directories(
        ${TOOLS_PATH}
        ${SRC_PATH}
        ${TOOLS_PATH}/glew/include
        ${SDL_INCLUDE_DIR}
    )
    add_executable(sdldemo ${SDLSRC})

    target_link_libraries(sdldemo 
        base
        renderer
        ${SDL_LIBRARY}
    )
endif()