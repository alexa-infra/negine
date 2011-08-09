set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)

set(GTEST_PATH ${TOOLS_PATH}/gtest)

add_source_group("tests"        ${TESTS_PATH}       TESTS_SRC)
add_source_group("google-test"  ${GTEST_PATH}       TESTS_SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/glew/include
)
add_executable(tests ${TESTS_SRC})

add_dependencies(tests
    glew
    base
    renderer
)

target_link_libraries(tests
    glew
    base
    renderer
)

if(UNIX)
  target_link_libraries(tests pthread)
endif(UNIX)

add_definitions(-DGLEW_STATIC)
