
set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)

add_source_group("src"      ${TOOLS_PATH}/glew/src          SRC)
add_source_group("include"  ${TOOLS_PATH}/glew/include/GL   SRC)

include_directories(${TOOLS_PATH}/glew/include)

add_library(glew STATIC ${SRC})

add_definitions(-DGLEW_STATIC)