# Trillek Engine C
[![Build Status](https://travis-ci.org/trillek-team/tec.svg)](https://travis-ci.org/trillek-team/tec)


## Building
TEC requires cmake 3.1 and a few libraries like GLEW, Bullet and OpenAL . Our CMake script could handle compiling and installing Bullet and OpenAL, but GLEW is necesary to be installed on your computer to build the engine properly.

If you not have cmake 3.1 (try `cmake -version`), you could install on Linux, using the same procedure that we do on Travis CI :
1. `(cd /tmp && wget http://www.cmake.org/files/v3.1/cmake-3.1.3.tar.gz && tar zxf cmake-3.1.3.tar.gz)`
2. `(cd /tmp/cmake-3.1.3 && cmake . && make && sudo make install)`
3. 
You should get not cmake 3.1.3 when you run `cmake -version`

Building takes a few steps to get everything set up for the first build.

1. `git submodule update --init` in the root directory.
2. `mkdir build/` in to root directory
3. `cd build`/
4. Follow platform specific instructions 
  1. Linux
     If you have Bullet, GLEW and OpenAL dev libs installed : 
       1. `cmake ..` in the build directory
       2. `make TEC` in the build directory
     If you not have Bullet or OpenAL dev libs installed :
       1. `cmake ..` in the build directory
       2. `make` in the build directory
       3. `sudo make install` in the build directory (Warning! Would install Bullet or OpenAL on your /usr/local/)
       4. `rm CMakeCache.txt` in the build directory
       5. `cmake ..` in the build directory
       6. `make TEC` in the build directory
  2. Windows
     1. Run cmake-gui setting the source line to the root directory and the build line to the build directory.
     2. Configure and Generate using non-x64 as the target, with native compiles selected.
     3. Build all dependencies in Release configuration (if it is just tec in the project list go to step 5).
     **You must build bullet in both debug and release configurations**
     4. Return to cmake-gui and rerun configure and generate. If you see "All dependencies found now building tec.",
     you can delete your cache and rerun to clean up the cmake-gui window. 
     5. In the project properties for `TEC` change the `Debugging`->`Working Directory` to `$(SolutionDir)..\`.
     6. Download and install oalinst.zip (OpenAL installer) http://openal.org/creative-installers/ and install it.
5. Run it from `tec/`
