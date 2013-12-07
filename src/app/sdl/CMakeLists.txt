
find_package(PythonLibs REQUIRED)
if (NOT PYTHONLIBS_FOUND)
    message(FATAL ERROR "Python is not found")
endif()

add_source_group("sdl" ${CMAKE_CURRENT_SOURCE_DIR} SRC)

include_directories(
    ${TOOLS_PATH}
    ${TOOLS_PATH}/SDL/include
    ${TOOLS_PATH}/boost-python
    ${NEGINE_SRC_PATH}
    ${PYTHON_INCLUDE_DIRS}
)

if(NEGINE_STATIC_BUILD)
    add_executable(sdl-app-static ${SRC})
    target_link_libraries(sdl-app-static SDL2-static negine-static boostpython-static)
    set_target_properties(sdl-app-static PROPERTIES
        COMPILE_FLAGS "-DBOOST_PYTHON_STATIC_LIB -DNEGINE_STATIC_BUILD"
    )
endif()

if(NEGINE_SHARED_BUILD)
    add_executable(sdl-app ${SRC})
    target_link_libraries(sdl-app SDL2 negine boostpython)
endif()

add_definitions(-DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_ALL_NO_LIB)

set(SIZEOF_VOIDP ${CMAKE_SIZEOF_VOID_P})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_64 TRUE)
else()
    set(ARCH_64 FALSE)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if(WIN32 AND ARCH_64 AND NOT MSVC)
    add_definitions(-DMS_WIN64)
endif()

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()

if(MSVC)
    if(NEGINE_STATIC_BUILD)
        configure_file(${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/sdl-app-static.vcxproj.user @ONLY)
    endif()
    if(NEGINE_SHARED_BUILD)
        configure_file(${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/sdl-app.vcxproj.user @ONLY)
    endif()
endif() 
