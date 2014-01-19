
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HDR ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
source_group("src" FILES ${HDR} ${SRC})

include_directories(
    ${TOOLS_PATH}
    ${NEGINE_SRC_PATH}
    ${TOOLS_PATH}/boost-python
    ${PYTHON_INCLUDE_DIRS}
)

if(NEGINE_STATIC_BUILD)
    add_executable(runtime-static ${SRC} ${HDR})
    target_link_libraries(runtime-static negine-static)
endif()

if(NEGINE_SHARED_BUILD)
    add_executable(runtime ${SRC} ${HDR})
    target_link_libraries(runtime negine)
endif()

if(MSVC)
    get_filename_component(PYTHON_LIB_PATH "${PYTHON_LIBRARY_RELEASE}" DIRECTORY)
    
    if(DEFINED PREBUILD_THIRDPARTY_PATH)
        set(CFG_NAME ${CMAKE_SOURCE_DIR}/cmake/project-prebuild.vcxproj.user.in)
    else()
        set(CFG_NAME ${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in)
    endif()
    
    if(NEGINE_STATIC_BUILD)
        configure_file(${CFG_NAME}
            ${CMAKE_CURRENT_BINARY_DIR}/runtime-static.vcxproj.user @ONLY)
    endif()
    if(NEGINE_SHARED_BUILD)
        configure_file(${CFG_NAME}
            ${CMAKE_CURRENT_BINARY_DIR}/runtime.vcxproj.user @ONLY)
    endif()
endif() 
