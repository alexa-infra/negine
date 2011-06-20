set(SRC_PATH ${negine_SOURCE_DIR})
set(BASE_PATH ${SRC_PATH}/base)
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)

set(GTEST_PATH ${TOOLS_PATH}/gtest)

add_source_group("tests"        ${TESTS_PATH}       TESTS_SRC)
add_source_group("base"         ${BASE_PATH}        TESTS_SRC)
add_source_group("base/math"    ${BASE_PATH}/math   TESTS_SRC)
add_source_group("google-test"  ${GTEST_PATH}       TESTS_SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
)
add_executable(tests ${TESTS_SRC})

target_link_libraries(tests pthread)
