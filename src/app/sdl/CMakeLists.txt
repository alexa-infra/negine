
add_source_group("sdl" ${CMAKE_CURRENT_SOURCE_DIR} SRC)

include_directories(
    ${TOOLS_PATH}
    ${TOOLS_PATH}/SDL/include
    ${NEGINE_SRC_PATH}
)

add_executable(sdl-app ${SRC})

if(NEGINE_STATIC_BUILD)
    target_link_libraries(sdl-app SDL2-static negine)
endif()

if(NEGINE_SHARED_BUILD)
    target_link_libraries(sdl-app SDL2 negine)
endif()