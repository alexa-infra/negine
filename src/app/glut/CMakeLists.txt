
add_source_group("glut" ${CMAKE_CURRENT_SOURCE_DIR} SRC)

include_directories(
    ${NEGINE_SRC_PATH}
    ${TOOLS_PATH}
    ${TOOLS_PATH}/freeglut/include
)

add_executable(glut-app ${SRC})

if(NEGINE_STATIC_BUILD)
    target_link_libraries(glut-app freeglut-static negine)
endif()

if(NEGINE_SHARED_BUILD)
    target_link_libraries(glut-app freeglut negine)
endif()

if(NEGINE_STATIC_BUILD)
    add_definitions(-DFREEGLUT_STATIC)
endif()

add_definitions(-DFREEGLUT_LIB_PRAGMAS=0)