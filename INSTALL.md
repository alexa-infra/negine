windows

Tools:
* cmake 2.8.12.1+ (http://www.cmake.org/cmake/resources/software.html)
* mingw-w64 from mingw-builds http://sourceforge.net/projects/mingwbuilds/files/host-windows/releases/4.8.1/64-bit/threads-posix/seh/
* MS Visual Studio 2013 (express/prof)
* python 3.3.3 (also it needs correct lib-import)

Create projects:
* cmake -G"Unix Makefiles" -DSHARED=ON path-to-sources
* cmake -G"Visual Studio 12 x64" -DSHARED=ON path-to-sources
* or -DSHARED=OFF for static build

linux/osx should be the similar