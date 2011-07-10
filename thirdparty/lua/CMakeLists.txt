set(SRC_PATH ${negine_SOURCE_DIR})
set(TOOLS_PATH ${SRC_PATH}/thirdparty)

add_source_group("src"      ${TOOLS_PATH}/lua/src          SRC)
add_source_group("include"  ${TOOLS_PATH}/lua/include      SRC)

include_directories(
	${TOOLS_PATH}/lua/include
)

add_library(lua STATIC ${SRC})

if(UNIX)
  add_definitions(-DLUA_USE_POSIX)
endif(UNIX)