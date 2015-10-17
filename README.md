# Trillek Engine C
| GNU/Linux (Gcc and CLang)                        | Windows (VStudio 2015)  |
|--------------------------------------------------|-------------------------|
|[![Build Status](https://travis-ci.org/trillek-team/tec.svg)](https://travis-ci.org/trillek-team/tec) | [![Build status](https://ci.appveyor.com/api/projects/status/n89l3qj4oo2v20th?svg=true)](https://ci.appveyor.com/project/Zardoz89/tec-ojmoq) |


## Building
TEC requires cmake 3.1 and a few libraries like GLEW, Lua, Bullet and OpenAL. Our CMake script can handle compiling and installing Bullet and OpenAL, but GLEW and Lua is necesary to be installed on your computer to build the engine properly.

If you do not have cmake 3.1 (try `cmake -version`), to install on Linux, use the same procedure that we do on Travis CI:

1. `(cd /tmp && wget http://www.cmake.org/files/v3.1/cmake-3.1.3.tar.gz && tar zxf cmake-3.1.3.tar.gz)`
2. `(cd /tmp/cmake-3.1.3 && cmake . && make && sudo make install)`
3. Close your terminal emulator and open a new instance. Now, you should get version 3.1.3 when you run `cmake -version`

Building takes a few steps to get everything set up for the first build.

1. `git submodule update --init` in the root directory.
2. `mkdir build/` in to root directory
3. `cd build/`
4. Follow platform specific instructions 
  1. Linux (G++ 3.9 or CLang 3.7)
     - If you have Bullet, GLEW and OpenAL dev libs installed : 
       1. `cmake ..` in the build directory
       2. `make TEC` in the build directory
     - If you do not have Bullet or OpenAL dev libs installed : 
       1. `cmake ..` in the build directory
       2. `make` in the build directory
       3. `sudo make install` in the build directory (Warning! This will install Bullet and/or OpenAL on your /usr/local/)
       4. `rm CMakeCache.txt` in the build directory
       5. `cmake ..` in the build directory
       6. `make TEC` in the build directory
  2. Windows (VStudio 2015)
     1. Install Lua include files and library file. (For example on /lib )
     2. Set LUA_DIR enviroment variable to the path were you download Lua.
     3. Run cmake-gui setting the source line to the root directory and the build line to the build directory.
     4. Configure and Generate using non-x64 as the target, with native compiles selected.
     5. Build all dependencies in Release configuration (if it is just tec in the project list go to step 5).
     **You must build bullet in both debug and release configurations**
     6. Return to cmake-gui and rerun configure and generate. If you see "All dependencies found now building tec.",
     you can delete your cache and rerun to clean up the cmake-gui window. 
     7. In the project properties for `TEC` change the `Debugging`->`Working Directory` to `$(SolutionDir)..\`.
     8. Download and install oalinst.zip (OpenAL installer) http://openal.org/downloads/ and install it.
5. Run it from `tec/`

### Depencies download (Windows)

- Glew : Use nuget -> nuget install rxd_glew 
- Lua 5.2 : http://sourceforge.net/projects/luabinaries/files/5.2.3/Windows%20Libraries/Static/

### Unit tests

To generate the unit tests, follow the same instructions that before, but set to true the flag BUILD_TESTS_TEC
