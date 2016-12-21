# Trillek Engine C
| Windows (VStudio 2015)  | Semaphore (GCC) |
|-------------------------|-----------------|
| [![Build status](https://ci.appveyor.com/api/projects/status/809xi9ukwo7sgsip?svg=true)](https://ci.appveyor.com/project/adam4813/tec-hem9u) | [![Build Status](https://semaphoreci.com/api/v1/adam4813/trillek-team-tec/branches/master/shields_badge.svg)](https://semaphoreci.com/adam4813/trillek-team-tec) |


## Building
TEC requires cmake 3.1 and a few libraries GLFW3, GLM, ASIO, Protobuf, GLEW, Lua, Bullet and OpenAL.

If you do not have cmake 3.1 (try `cmake -version`), to install on Linux, use the same procedure that we do on Travis CI:

1. `(cd /tmp && wget http://www.cmake.org/files/v3.1/cmake-3.1.3.tar.gz && tar zxf cmake-3.1.3.tar.gz)`
2. `(cd /tmp/cmake-3.1.3 && cmake . && make && sudo make install)`
3. Close your terminal emulator and open a new instance. Now, you should get version 3.1.3 when you run `cmake -version`

Building takes a few steps to get everything set up for the first build.

1. `git submodule update --init` in the root directory.
2. `mkdir build/` in to root directory
3. `cd build/`
4. Follow platform specific instructions 
  1. Linux (G++ 5 or CLang 3.8)
       1. **INSTALL REQUIRED LIBS** bullet, glew, glfw3, glm, asio, lua, openal-soft, and protobuf. Some of these will need versions not in your distribution (just ask for help in the IRC.)
            1. If you are on Ubuntu/Debian/etc. (something with `apt`):
                1. Run `apt-get install libglew-dev libglfw3 libglm-dev libasio-dev`
                2. Run `apt-get install liblua5.2-dev libopenal-dev  libbullet-dev`
                3. Run `apt-get install libprotobuf-dev protobuf-compiler`
            2. If you are on Fedora/RHEL/etc. (something with `rpm`):
			    1. *instructions coming soon to a README near you*
            3. If you are on Arch/etc. (something with `pacman`):
			    1. Run `pacman -S glew glfw-x11 glm asio lua52 openal bullet protobuf`
       2. `cmake ..` in the build directory
       3. `make tec` in the build directory
  2. Windows (Visual Studio 2015)
     1. Check out **VCPKG** from https://github.com/Microsoft/vcpkg (If you already have **VCPKG**, go to step 5)
     2. Navigate to the `vcpkg/` directory.
     3. Run `powershell -exec bypass scripts\bootstrap.ps1`
     4. Run `.\vcpkg integrate install`
     5. Run `.\vcpkg install asio bullet3 glew glfw3 glm lua openal-soft protobuf zlib`
     6. Run cmake-gui setting the source line to the root directory and the build line to the build directory.
     7. Hit configure and select `Specify toolchain file for cross-compiling` using `scripts/buildsystems/vcpkg.cmake` from the the `vcpkg/` directory
     8. Click generate; then open and build the solution in Visual Studio.
     9. In the project properties for `trillek-client` change the `Debugging`->`Working Directory` to `$(SolutionDir)..\`.
     10. **Potentially** Download and install oalinst.zip (OpenAL installer) http://openal.org/downloads/ and install it.
5. Run it from `tec/`

### Unit tests
To generate the unit tests, follow the same instructions that before, but set to true the flag BUILD_TESTS_TEC
