
set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)

add_source_group("src"      ${TOOLS_PATH}/jsoncpp/src       SRC)
add_source_group("include"  ${TOOLS_PATH}/jsoncpp/include   SRC)

include_directories(${TOOLS_PATH}/jsoncpp/include)

add_library(jsoncpp STATIC ${SRC})

add_definitions(-DJSON_USE_EXCEPTION=0)
