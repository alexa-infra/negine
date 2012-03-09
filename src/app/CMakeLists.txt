set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

set(APP_PATH ${SRC_PATH}/app)

set(SRC
    ${APP_PATH}/demo.h
    ${APP_PATH}/demo.cpp
    ${APP_PATH}/luavm.cpp
    ${APP_PATH}/main.cpp
)
if (USE_SDL)
    list(APPEND SRC ${APP_PATH}/sdlapp.cpp)
elseif (USE_GLUT)
    list(APPEND SRC ${APP_PATH}/glutapp.cpp)    
endif()
source_group("app" FILES ${SRC})

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/lua/include
)

add_definitions(-DGLEW_STATIC)

add_executable(app
    ${SRC}
)
add_dependencies(app
    negine
)
target_link_libraries(app 
    negine
)

if (USE_SDL)
    find_package(SDL 1.3) 
    if(NOT SDL_FOUND)
        message(FATAL_ERROR "SDL is not found")
    endif()

    add_definitions(-DSDL_ENGINE)

    include_directories(
        ${SDL_INCLUDE_DIR}
    )
    target_link_libraries(app 
        ${SDL_LIBRARY}
    )
elseif (USE_GLUT)
    add_definitions(-DGLUT_ENGINE)
    add_definitions(-DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0)
    include_directories(
        ${TOOLS_PATH}/freeglut/include
    )
    target_link_libraries(app 
        freeglut
    )
endif()
