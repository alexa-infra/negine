set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

set(APP_PATH ${SRC_PATH}/app)

#add_source_group("app" ${APP_PATH} SRC)
if (USE_SDL)
    add_source_group("sdl" ${APP_PATH}/sdl SRC)
elseif (USE_GLUT)
    add_source_group("glut" ${APP_PATH}/glut SRC)
endif()

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/lua/include
)

add_definitions(-DGLEW_STATIC)

add_library(app
    ${SRC}
)
add_dependencies(app
    negine
)
target_link_libraries(app 
    negine
)

if (USE_SDL)
    find_package(SDL2) 
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
	if(WIN32)
		target_link_libraries(app winmm.lib)
	endif()
endif()
