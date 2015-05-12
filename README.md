# Trillek Engine C

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
    1. Run the cmake-gui setting the source line to the root directory and the build line to the build directory.
    2. Configure and Generate using non-x64 as the target with natrive compiles selected.
    3. Build everything (an error about missing bullet libs, for `TEC`, is ok at this point).
    4. Return to cmake-gui and rerun configure and generate so Bullet can now find the libraries you just built.
    5. In the project properties for `TEC` change the `Debugging`->`Working Directory` to `$(SolutionDir)..\`.
5. Run it
