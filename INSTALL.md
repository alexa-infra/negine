How to run:

windows (tested on win8 x64):

# install tools
## mercurial 2.5.2+ command line tools or tortoiseHG (http://mercurial.selenic.com/downloads/)
## cmake 2.8.10.2+ (http://www.cmake.org/cmake/resources/software.html)
## gow (https://github.com/bmatzelle/gow)
## make sure mercurial, cmake, gow binary folders are in your PATH variable
# get mingw-w64
## download mingw-w64 from mingw-builds http://sourceforge.net/projects/mingwbuilds/files/host-windows/releases/4.8.0/64-bit/threads-posix/seh/x64-4.8.0-release-posix-seh-rev2.7z/download
## unpack to c:/devtools/mingw64
## add mingw bin directory to your PATH environment variable, e.g. c:/devtools/mingw64/bin
# checkout SDL2
## tested with 7250:bbb6b079cfe5 revision (bbb6b079cfe51ec1320fe5203723600f79b0f0d4)
## cd c:/devtools/
## hg clone http://hg.libsdl.org/SDL
## cd c:/devtools/SDL
## hg update -r 7250
# build and install SDL
## mkdir c:/devtools/SDL-build
## cd c:/devtools/SDL-build
## cmake -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=c:/devtools/SDL-build/build c:/devtools/SDL
## mingw32-make
## mingw32-make install
## create new environment variable SDLDIR with value c:/devtools/SDL-build/build
# checkout negine
## hg or git ...
## mkdir c:/projects/negine/_build
## cd c:/projects/negine/_build
## hg -G"Unix Makefiles" -DUSE_SDL=True -DUSE_GLUT=False c:/projects/negine/
## mingw32-make
## cp %SDLDIR%/libSDL2.dll c:/projects/negine/_build
# copy resources and run
## cp -r c:/projects/negine/data/* c:/projects/negine/_build/
## cd c:/projects/negine/_build
## ./demos/particles_demo.exe
## ./demos/md5model_demo.exe