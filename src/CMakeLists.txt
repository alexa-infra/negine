set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(PythonLibs REQUIRED)
if (NOT PYTHONLIBS_FOUND)
    message(FATAL ERROR "Python is not found")
endif()

file(GLOB BASE_SRC ${SRC_PATH}/base/*.cpp)
file(GLOB BASE_HDR ${SRC_PATH}/base/*.h)
source_group("base" FILES ${BASE_SRC} ${BASE_HDR})

file(GLOB MATH_SRC ${SRC_PATH}/math/*.cpp)
file(GLOB MATH_HDR ${SRC_PATH}/math/*.h)
source_group("math" FILES ${MATH_SRC} ${MATH_HDR})

file(GLOB RENDER_SRC ${SRC_PATH}/render/*.cpp)
file(GLOB RENDER_HDR ${SRC_PATH}/render/*.h)
source_group("render" FILES ${RENDER_SRC} ${RENDER_HDR})

file(GLOB APP_SRC ${SRC_PATH}/app/*.cpp)
file(GLOB APP_HDR ${SRC_PATH}/app/*.h)

file(GLOB SDLAPP_SRC ${SRC_PATH}/app/sdl/*.cpp)
file(GLOB SDLAPP_HDR ${SRC_PATH}/app/sdl/*.h)
source_group("app" FILES ${APP_SRC} ${APP_HDR} ${SDLAPP_SRC} ${SDLAPP_HDR})

set(SRC ${APP_SRC} ${SDLAPP_SRC} ${BASE_SRC} ${MATH_SRC} ${RENDER_SRC})
set(HDR ${APP_HDR} ${SDLAPP_HDR} ${BASE_HDR} ${MATH_HDR} ${RENDER_HDR})

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -Wno-invalid-offsetof -std=gnu++11 -fvisibility=hidden")
endif()
if(APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") 
    set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework OpenGL")
endif()
if(WIN32 AND MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
endif()
set(CMAKE_DEBUG_POSTFIX "_d")

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${TOOLS_PATH}/boost-python
    ${PYTHON_INCLUDE_DIRS}
    ${TOOLS_PATH}/SDL/include
)

if(NEGINE_STATIC_BUILD)
    add_library(negine-static STATIC
        ${SRC} ${HDR}
    )
    set_target_properties(negine-static PROPERTIES
        OUTPUT_NAME "negine"
        COMPILE_FLAGS "-DNEGINE_SOURCE"
        )
    add_definitions(-DBOOST_PYTHON_STATIC_LIB -DNEGINE_STATIC_BUILD -DBOOST_PYTHON_STATIC_MODULE)
    target_link_libraries(negine-static boostpython-static SDL2-static)
endif()
if(NEGINE_SHARED_BUILD)
    add_library(negine SHARED
        ${SRC} ${HDR}
    )
    set_target_properties(negine PROPERTIES
        OUTPUT_NAME "negine"
        COMPILE_FLAGS "-DNEGINE_SOURCE"
        )
    add_definitions(-DNEGINE_SHARED_BUILD)
    target_link_libraries(negine boostpython SDL2)
endif()

add_definitions(-DSDL_ENGINE)
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

add_subdirectory(runtime)