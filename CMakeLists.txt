cmake_minimum_required(VERSION 2.8)
project(nengine)

set(SRC_PATH ${nengine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)

add_subdirectory(${TESTS_PATH} ${PROJECT_BINARY_DIR}/tests)
