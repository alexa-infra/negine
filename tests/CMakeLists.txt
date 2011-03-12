set(SRC_PATH ${nengine_SOURCE_DIR})
set(BASE_PATH ${SRC_PATH}/base)
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)

file(GLOB_RECURSE TESTS_SRC
     ${TESTS_PATH}/*.h
     ${TESTS_PATH}/*.cpp
)
source_group("tests" FILES ${TESTS_SRC})

file(GLOB_RECURSE BASE_SRC
     ${BASE_PATH}/*.h
     ${BASE_PATH}/*.cpp
)
source_group("base" FILES ${BASE_SRC})
list(APPEND TESTS_SRC ${BASE_SRC})

set(GTEST_PATH ${TOOLS_PATH}/gtest)
file(GLOB_RECURSE GTEST_SRC
    ${GTEST_PATH}/*.h
    ${GTEST_PATH}/*.cpp
    ${GTEST_PATH}/*.cc
)
source_group("google-test" FILES ${GTEST_SRC})
list(APPEND TESTS_SRC ${GTEST_SRC})

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
)
add_executable(Tests ${TESTS_SRC})

target_link_libraries(Tests pthread)
