set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(Python_ADDITIONAL_VERSIONS 3.3)
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

file(GLOB ENGINE_SRC ${SRC_PATH}/engine/*.cpp)
file(GLOB ENGINE_HDR ${SRC_PATH}/engine/*.h)
source_group("engine" FILES ${ENGINE_SRC} ${ENGINE_HDR})

file(GLOB APP_SRC ${SRC_PATH}/app/*.cpp)
file(GLOB APP_HDR ${SRC_PATH}/app/*.h)

file(GLOB SDLAPP_SRC ${SRC_PATH}/app/sdl/*.cpp)
file(GLOB SDLAPP_HDR ${SRC_PATH}/app/sdl/*.h)
source_group("app" FILES ${APP_SRC} ${APP_HDR} ${SDLAPP_SRC} ${SDLAPP_HDR})

file(GLOB GAME_SRC ${SRC_PATH}/game/*.cpp)
file(GLOB GAME_HDR ${SRC_PATH}/game/*.h)
file(GLOB GAME_COMP_SRC ${SRC_PATH}/game/components/*.cpp)
file(GLOB GAME_COMP_HDR ${SRC_PATH}/game/components/*.h)
source_group("game" FILES ${GAME_HDR} ${GAME_SRC} ${GAME_COMP_HDR} ${GAME_COMP_SRC})

file(GLOB PHYS_SRC ${SRC_PATH}/physics/*.cpp)
file(GLOB PHYS_HDR ${SRC_PATH}/physics/*.h)
source_group("physics" FILES ${PHYS_HDR} ${PHYS_SRC})

set(SRC ${APP_SRC} ${SDLAPP_SRC} ${BASE_SRC} ${MATH_SRC} ${RENDER_SRC} ${GAME_SRC} ${GAME_COMP_SRC} ${ENGINE_SRC} ${PHYS_SRC})
set(HDR ${APP_HDR} ${SDLAPP_HDR} ${BASE_HDR} ${MATH_HDR} ${RENDER_HDR} ${GAME_HDR} ${GAME_COMP_HDR} ${ENGINE_HDR} ${PHYS_HDR})

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -Wno-invalid-offsetof -std=gnu++11 -fvisibility=hidden")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
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

if(DEFINED PREBUILD_THIRDPARTY_PATH)
    include_directories(
        ${PREBUILD_THIRDPARTY_PATH}/include
        ${PREBUILD_THIRDPARTY_PATH}/include/SDL2
    )
    link_directories(${PREBUILD_THIRDPARTY_PATH}/lib)
else()
    include_directories(
        ${SRC_PATH}
        ${TOOLS_PATH}/boost-python
        ${TOOLS_PATH}/SDL/include
        ${TOOLS_PATH}/assimp-3.0.1270/include
        ${TOOLS_PATH}/bitsquid
        ${TOOLS_PATH}/bullet-2.82-r2704/src
    )
endif()

include_directories(
    ${TOOLS_PATH}
    ${SRC_PATH}
    ${PYTHON_INCLUDE_DIRS}
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
    target_link_libraries(negine-static boostpython-static SDL2-static assimp-static bitsquid-foundation BulletDynamics LinearMath BulletCollision BulletSoftBody)
    if(DEFINED PREBUILD_THIRDPARTY_PATH)
        target_link_libraries(negine-static
            debug ${PYTHON_LIBRARY_DEBUG}
            optimized ${PYTHON_LIBRARY_RELEASE}
        )
    endif()
endif()
if(NEGINE_SHARED_BUILD)
    add_library(negine SHARED
        ${SRC} ${HDR}
    )
    set_target_properties(negine PROPERTIES
        OUTPUT_NAME "negine"
        COMPILE_FLAGS "-DNEGINE_SOURCE"
        )
    add_definitions(-DNEGINE_SHARED_BUILD -DASSIMP_DLL)
    target_link_libraries(negine boostpython SDL2 assimp bitsquid-foundation BulletDynamics LinearMath BulletCollision BulletSoftBody)
    if(DEFINED PREBUILD_THIRDPARTY_PATH)
        target_link_libraries(negine
            debug ${PYTHON_LIBRARY_DEBUG}
            optimized ${PYTHON_LIBRARY_RELEASE}
        )
    endif()
endif()

add_definitions(-DSDL_ENGINE)
add_definitions(-DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_ALL_NO_LIB)

if(WIN32)
    add_definitions(-DMS_NO_COREDLL -DPy_ENABLE_SHARED)
endif()

set(SIZEOF_VOIDP ${CMAKE_SIZEOF_VOID_P})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_64 TRUE)
else()
    set(ARCH_64 FALSE)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if(WIN32 AND ARCH_64 AND NOT MSVC)
    add_definitions(-DMS_WIN64)
endif()

if(WIN32)
    add_definitions(-DWIN32)
endif()

if(MSVC)
    add_definitions(-DHAVE_ROUND)
endif()

if (NOT ${PYTHON_LIBRARY_DEBUG} STREQUAL ${PYTHON_LIBRARY_RELEASE})
    add_definitions(-DBOOST_DEBUG_PYTHON)
endif()

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()

if(NEGINE_STATIC_BUILD)
    #add_subdirectory(tests)
endif()

add_subdirectory(runtime)