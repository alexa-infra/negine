set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(PythonLibs REQUIRED)
if (NOT PYTHONLIBS_FOUND)
    message(FATAL ERROR "Python is not found")
endif()

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
    set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /WX /EHsc")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
endif()
set(CMAKE_DEBUG_POSTFIX "_d")

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/boost-python
    ${PYTHON_INCLUDE_DIRS}
)

if(NEGINE_STATIC_BUILD)
    add_library(negine-static STATIC
        ${MATH_SRC}
        ${BASE_SRC}
        ${RENDER_SRC}
    )
    set_target_properties(negine-static PROPERTIES
        OUTPUT_NAME "negine"
        )
    add_definitions(-DBOOST_PYTHON_STATIC_LIB -DNEGINE_STATIC_BUILD)
    target_link_libraries(negine-static boostpython-static)
endif()
if(NEGINE_SHARED_BUILD)
    add_library(negine SHARED
        ${MATH_SRC}
        ${BASE_SRC}
        ${RENDER_SRC}
    )
    set_target_properties(negine PROPERTIES
        OUTPUT_NAME "negine"
        )
    add_definitions(-DNEGINE_SHARED_BUILD)
    target_link_libraries(negine boostpython)
endif()

add_definitions(-DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_ALL_NO_LIB)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_64 TRUE)
else()
    set(ARCH_64 FALSE)
endif()

if(WIN32 AND ARCH_64 AND NOT MSVC)
    add_definitions(-DMS_WIN64)
endif()

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()

if(NEGINE_STATIC_BUILD)
    #add_subdirectory(tests)
endif()
add_subdirectory(app)