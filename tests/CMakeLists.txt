set(SRC_PATH ${negine_SOURCE_DIR})
set(BASE_PATH ${SRC_PATH}/base)
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)
set(GLEW_PATH ${TOOLS_PATH}/GL)

set(GTEST_PATH ${TOOLS_PATH}/gtest)

add_source_group("tests"        ${TESTS_PATH}       TESTS_SRC)
add_source_group("base"         ${BASE_PATH}        TESTS_SRC)
add_source_group("base/math"    ${BASE_PATH}/math   TESTS_SRC)
add_source_group("google-test"  ${GTEST_PATH}       TESTS_SRC)
add_source_group("renderer"     ${SRC_PATH}/renderer TESTS_SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
    ${TOOLS_PATH}/freeglut/include
)
add_executable(tests ${TESTS_SRC})

add_dependencies(tests
    freeglut
    glew
)

target_link_libraries(tests
    freeglut
    glew
)

if(WIN32)
  target_link_libraries(tests OpenGL32 winmm user32)
endif(WIN32)
if(UNIX)
  target_link_libraries(tests pthread GL)
endif(UNIX)

set_target_properties(tests PROPERTIES
    COMPILE_FLAGS "-DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0 -DGLEW_STATIC"
)
