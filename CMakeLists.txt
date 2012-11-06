cmake_minimum_required(VERSION 2.8)
project(negine)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(SRC_PATH ${negine_SOURCE_DIR}/src)
set(TOOLS_PATH ${negine_SOURCE_DIR}/thirdparty)
set(TESTS_PATH ${SRC_PATH}/tests)

option(USE_SDL "SDL" ON)
option(USE_GLUT "Freeglut" OFF)

if (USE_SDL AND USE_GLUT)
  message(FATAL_ERROR "use only one sdl or glut option")
endif()
if (NOT USE_SDL AND NOT USE_GLUT)
  message(FATAL_ERROR "use one engine: sdl or glut")
endif()

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

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Werror -fmax-errors=1 -std=c++98")
  add_definitions(-D_DEBUG)
endif(UNIX)
if(APPLE)
  option(BUILD_FOR_MACOS "MacOS" ON)
  option(BUILD_FOR_IOS   "iOS"   OFF)
  
  if (BUILD_FOR_MACOS)
    set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework OpenGL")
  endif(BUILD_FOR_MACOS)
  
  if (BUILD_FOR_IOS)
    
    add_source_group("sources"         ${SRC_PATH}       SOURCES)

    # message("My sources: ${SOURCES}")
    
    file(GLOB_RECURSE SOURCES
        ${SRC_PATH}/*.h
        ${SRC_PATH}/*.cpp
        ${SRC_PATH}/*.cc
        ${SRC_PATH}/*.c
    )

    set(IOS_FRAMEWORKS
        Foundation
        AudioToolbox
        CoreGraphics
        QuartzCore
        UIKit
        OpenGLES
    )
  
    set(CMAKE_OSX_SYSROOT iphonesimulator5  .0)
    set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_BIT))
    
    foreach(FW ${IOS_FRAMEWORKS})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework ${FW}")
    endforeach()
    
    # link_directories(\${HOME}/\${SDKROOT}/lib)

    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.mycompany.\${PRODUCT_NAME:identifier}")

    

    add_executable(
    negineApp
    MACOSX_BUNDLE
    ${SOURCES}
    )

    target_link_libraries(negineApp SDL)

    # code signing
    # set_target_properties(${NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer: My Name")
  endif (BUILD_FOR_IOS)
  
endif(APPLE)
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -WX")
endif(WIN32)
set(CMAKE_DEBUG_POSTFIX "_d")

if(USE_SDL)
    add_definitions(-DSDL_ENGINE)
elseif(USE_GLUT)
    add_definitions(-DGLUT_ENGINE)
endif()

link_directories(${TOOLS_PATH}/libs-win-vs2012)

#add_subdirectory(${TOOLS_PATH}/lua ${PROJECT_BINARY_DIR}/lua)
#add_subdirectory(${TOOLS_PATH}/jsoncpp ${PROJECT_BINARY_DIR}/jsoncpp)

add_subdirectory(${SRC_PATH} ${PROJECT_BINARY_DIR}/negine)
add_subdirectory(${SRC_PATH}/app ${PROJECT_BINARY_DIR}/app)
add_subdirectory(${TESTS_PATH} ${PROJECT_BINARY_DIR}/tests)

add_subdirectory(${SRC_PATH}/demos ${PROJECT_BINARY_DIR}/demos)
