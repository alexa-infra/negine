
set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)

add_source_group("src"      ${TOOLS_PATH}/freeglut/src          SRC)
add_source_group("include"  ${TOOLS_PATH}/freeglut/include/GL   SRC)

include_directories(${TOOLS_PATH}/freeglut/include ${TOOLS_PATH}/freeglut/src)

add_library(freeglut STATIC ${SRC})

set_target_properties(freeglut PROPERTIES
    COMPILE_FLAGS "-DFREEGLUT_STATIC -DFREEGLUT_LIB_PRAGMAS=0"
)


