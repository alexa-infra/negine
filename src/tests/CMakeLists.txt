set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)

set(GTEST_PATH ${TOOLS_PATH}/gtest)

add_source_group("tests"        ${TESTS_PATH}       TESTS_SRC)
add_source_group("google-test"  ${GTEST_PATH}       TESTS_SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
)
add_executable(tests ${TESTS_SRC})

add_dependencies(tests
    negine
)

target_link_libraries(tests
    negine
)

if(UNIX)
  target_link_libraries(tests pthread)
endif()

if(MINGW)
  add_definitions(-DGTEST_HAS_PTHREAD=0)
endif()

if(MSVC)
	add_definitions(-D_VARIADIC_MAX=10)
endif()
