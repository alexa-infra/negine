if(NOT NEGINE_STATIC_BUILD)
	set(NEGINE_STATIC_BUILD OFF)
endif()

if(NOT NEGINE_SHARED_BUILD)
	set(NEGINE_SHARED_BUILD OFF)
endif()

set(SDL_STATIC ${NEGINE_STATIC_BUILD} CACHE BOOL "SDL static build" FORCE)
set(BOOSTPYTHON_STATIC ${NEGINE_STATIC_BUILD} CACHE BOOL "Boost python static build" FORCE)
set(FREEGLUT_STATIC ${NEGINE_STATIC_BUILD} CACHE BOOL "Freeglut static build" FORCE)

set(SDL_SHARED ${NEGINE_SHARED_BUILD} CACHE BOOL "SDL shared build" FORCE)
set(BOOSTPYTHON_SHARED ${NEGINE_SHARED_BUILD} CACHE BOOL "Boost python shared build" FORCE)
set(FREEGLUT_SHARED ${NEGINE_SHARED_BUILD} CACHE BOOL "Freeglut shared build" FORCE)

add_subdirectory(boost-python)
add_subdirectory(freeglut)
add_subdirectory(SDL)