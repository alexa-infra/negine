set(SRC_PATH ${negine_SOURCE_DIR})
set(BASE_PATH ${SRC_PATH}/base)
set(TOOLS_PATH ${SRC_PATH}/thirdparty)

add_source_group("base"         ${BASE_PATH}        SRC)
add_source_group("base/math"    ${BASE_PATH}/math   SRC)

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
)
add_library(base ${SRC})