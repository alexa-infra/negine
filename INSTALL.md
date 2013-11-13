How to run:

windows - mingw (tested on win8 x64):

# install tools
## mercurial 2.5.2+ command line tools or tortoiseHG (http://mercurial.selenic.com/downloads/)
## cmake 2.8.10.2+ (http://www.cmake.org/cmake/resources/software.html)
## gow (https://github.com/bmatzelle/gow)
## make sure mercurial, cmake, gow binary folders are in your PATH variable
# get mingw-w64
## download mingw-w64 from mingw-builds http://sourceforge.net/projects/mingwbuilds/files/host-windows/releases/4.8.0/64-bit/threads-posix/seh/x64-4.8.0-release-posix-seh-rev2.7z/download
## unpack to c:/devtools/mingw64
## add mingw bin directory to your PATH environment variable, e.g. c:/devtools/mingw64/bin
# checkout negine
## hg or git ...
## mkdir c:/projects/negine/_build
## cd c:/projects/negine/_build
## hg -G"Unix Makefiles" c:/projects/negine/
## mingw32-make

almost similar things are needed to build for xcode and visual studio