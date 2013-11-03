set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_source_group("base"         ${SRC_PATH}/base        BASE_SRC)
add_source_group("math"         ${SRC_PATH}/math        MATH_SRC)
add_source_group("render"       ${SRC_PATH}/render      RENDER_SRC)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -Wno-invalid-offsetof -std=gnu++11 -fvisibility=hidden")
    add_definitions(-D_DEBUG)
endif()
if(APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") 
    set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework OpenGL")
endif()
if(WIN32 AND MSVC)
    set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /WX /GR-")
    add_definitions(-D_HAS_EXCEPTIONS=0)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
endif()
set(CMAKE_DEBUG_POSTFIX "_d")

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
)

if(NEGINE_STATIC_BUILD)
    add_library(negine-static STATIC
        ${MATH_SRC}
        ${BASE_SRC}
        ${RENDER_SRC}
    )
    set_target_properties(negine-static PROPERTIES
        COMPILE_FLAGS "-DNEGINE_STATIC_BUILD"
        OUTPUT_NAME "negine"
        )
endif()
if(NEGINE_SHARED_BUILD)
    add_library(negine SHARED
        ${MATH_SRC}
        ${BASE_SRC}
        ${RENDER_SRC}
    )
    set_target_properties(negine PROPERTIES
        COMPILE_FLAGS "-DNEGINE_SHARED_BUILD"
        OUTPUT_NAME "negine"
        )
endif()

if(NEGINE_STATIC_BUILD)
    add_subdirectory(tests)
endif()
add_subdirectory(app)