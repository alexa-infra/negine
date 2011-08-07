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
    ${Boost_INCLUDE_DIRS}
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
    ${Boost_LIBRARIES}
)

add_definitions(-DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0 -DGLEW_STATIC)

find_package(SDL 1.3) 
if(NOT SDL_FOUND)
    message("SDL is not found")
endif()

set(SDLSRC
    ${APP_PATH}/sdldemo.cpp
)
source_group("sdlapp" FILES ${SDLSRC})
include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/lua/include
    ${SDL_INCLUDE_DIR}
)
add_executable(sdldemo ${SDLSRC})

target_link_libraries(sdldemo 
    base
    renderer
    lua
    ${SDL_LIBRARY}
)
