
macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)
    file(GLOB TEMP_SRC
        ${SOURCE_PATH}/*.h
        ${SOURCE_PATH}/*.cpp
        ${SOURCE_PATH}/*.cc
        ${SOURCE_PATH}/*.c
    )
    source_group(${FILTER_NAME} FILES ${TEMP_SRC})
    list(APPEND ${TARGET_LIST} ${TEMP_SRC})
endmacro(add_source_group)

macro(create_demo NAME PATH)
    set(SRC_PATH ${negine_SOURCE_DIR}/src)
    set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)

    set(SRC "")

    include_directories(
        ${TOOLS_PATH}
        ${SRC_PATH}
        ${TOOLS_PATH}/glew/include
    )

    add_source_group("Sources" ${PATH} SRC)

    add_executable(${NAME}
        ${SRC}
    )

    add_dependencies(${NAME}
        negine
        app
    )

    target_link_libraries(${NAME}
        negine
        app
    )
endmacro()
