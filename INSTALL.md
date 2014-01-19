windows

Tools:
* cmake 2.8.12.1+ (http://www.cmake.org/cmake/resources/software.html)
* mingw-w64 from mingw-builds http://sourceforge.net/projects/mingwbuilds/files/host-windows/releases/4.8.1/64-bit/threads-posix/seh/
* MS Visual Studio 2013 (express/prof)
* python 3.3.3 (also it needs correct lib-import)

Create projects:
* cmake -G"Unix Makefiles" path-to-sources
* cmake -G"Visual Studio 12 Win64" path-to-sources
* or -DSHARED=OFF for static build

linux/osx should be the similar

Create project with prebuild thirdparty libraries:
* cd d:/projects/negine/_build/thirdparty
* cmake -G"Visual Studio 12 Win64" -DTHIRDPARTY_ONLY=ON -DCMAKE_INSTALL_PREFIX=d:/projects/negine/_build/prebuild d:/projects/negine/
* cmake --target install --config Debug --build d:/projects/negine/_build/thirdparty
* cd d:/projects/negine/_build/
* cmake -G"Visual Studio 12 Win64" -DPREBUILD_THIRDPARTY_PATH=d:/projects/negine/_build/prebuild d:/projects/negine/
* cmake --target build_all --config Debug --build d:/projects/negine/_build
