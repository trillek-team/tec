# Trillek Engine C
[![Build Status](https://travis-ci.org/trillek-team/tec.svg)](https://travis-ci.org/trillek-team/tec)


## Building
Building takes a few steps to get everything set up for the first build.

1. `git submodule update --init` in the root directory.
2. `mkdir build/` in to root directory
3. `cd build`/
4. Follow platform specific instructions
  1. Linux
     1. `cmake ..` in the build directory
     2. `make TEC` in the build directory
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
