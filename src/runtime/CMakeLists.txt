
add_source_group("sdl" ${CMAKE_CURRENT_SOURCE_DIR} SRC)

include_directories(
    ${TOOLS_PATH}
    ${TOOLS_PATH}/SDL/include
    ${TOOLS_PATH}/boost-python
    ${NEGINE_SRC_PATH}
    ${PYTHON_INCLUDE_DIRS}
)

if(NEGINE_STATIC_BUILD)
    add_executable(sdl-app-static ${SRC})
    target_link_libraries(sdl-app-static SDL2-static negine-static)
endif()

if(NEGINE_SHARED_BUILD)
    add_executable(sdl-app ${SRC})
    target_link_libraries(sdl-app SDL2 negine)
endif()

if(MSVC)
    if(NEGINE_STATIC_BUILD)
        configure_file(${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/sdl-app-static.vcxproj.user @ONLY)
    endif()
    if(NEGINE_SHARED_BUILD)
        configure_file(${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/sdl-app.vcxproj.user @ONLY)
    endif()
endif() 
