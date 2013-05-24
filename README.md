negine
======

Crossplatform OpenGL engine for my needs (experiments, concepts, discovering, and learning). Code is a product of entropy and chaos, it maybe broken (definitly), do not try to build/run it in current state. Tree structure is changing from time to time, and forking is not suggested.

License: MIT

Build system - cmake 2.8+

Supported platforms:
* win7/win8 32/64
* ubuntu 11.04
* macosx mountain lion

Tested build with:
* mingw32 gcc 4.7+
* clang 3.2 (on ubuntu/macosx)
* xcode 4+ with clang
* vs2012 express/prof (note: due to poor c++11 support in visual studio, build could be broken atm, and mingw+gcc is recommended to build under windows)

Thirdparty:
* SDL - v2, unstable version from trunk, source is included in thirdparty/SDL. need to build and install, or use some pre-build libraries (could be found in thirdparty/libs-win*).
* freeglut - source code is included, need to build and install (or use pre-build from thirdparty/libs-win*)
* google-testing-framework - is included to build, no need to install
* stb_image - is included to build, no need to install

Happy coding :D
