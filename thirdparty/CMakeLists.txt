if(NOT NEGINE_STATIC_BUILD)
	set(NEGINE_STATIC_BUILD OFF)
endif()

if(NOT NEGINE_SHARED_BUILD)
	set(NEGINE_SHARED_BUILD OFF)
endif()

set(SDL_STATIC ${NEGINE_STATIC_BUILD} CACHE BOOL "SDL static build" FORCE)
set(BOOSTPYTHON_STATIC ${NEGINE_STATIC_BUILD} CACHE BOOL "Boost python static build" FORCE)
set(FREEGLUT_STATIC ${NEGINE_STATIC_BUILD} CACHE BOOL "Freeglut static build" FORCE)
set(BUILD_STATIC_LIB ${NEGINE_STATIC_BUILD} CACHE BOOL "Assimp static build" FORCE)

set(SDL_SHARED ${NEGINE_SHARED_BUILD} CACHE BOOL "SDL shared build" FORCE)
set(BOOSTPYTHON_SHARED ${NEGINE_SHARED_BUILD} CACHE BOOL "Boost python shared build" FORCE)
set(FREEGLUT_SHARED ${NEGINE_SHARED_BUILD} CACHE BOOL "Freeglut shared build" FORCE)

set(SDL_JOYSTICK OFF)
set(SDL_HAPTIC OFF)
set(ENABLE_BOOST_WORKAROUND ON CACHE BOOL "Enable Boost in assimp" FORCE)
set(BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Assimp tools" FORCE)

add_subdirectory(boost-python)
add_subdirectory(freeglut)
add_subdirectory(SDL)
add_subdirectory(assimp-3.0.1270)