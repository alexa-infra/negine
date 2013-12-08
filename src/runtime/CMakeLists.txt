
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
    if(NEGINE_STATIC_BUILD)
        configure_file(${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/runtime-static.vcxproj.user @ONLY)
    endif()
    if(NEGINE_SHARED_BUILD)
        configure_file(${CMAKE_SOURCE_DIR}/cmake/project.vcxproj.user.in
            ${CMAKE_CURRENT_BINARY_DIR}/runtime.vcxproj.user @ONLY)
    endif()
endif() 
