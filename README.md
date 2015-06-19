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
	3. Download and install oalinst.zip (OpenAL installer) http://openal.org/creative-installers/ and install it.
        1. Download http://kcat.strangesoft.net/openal-soft-1.16.0-bin.zip (OpenAL-soft SDK binaries) and extract somewhere.
		2. Copy the include folder to `tec/lib/include/` so al.h should be at `tec/lib/include/include/AL/al.h`.
		3. Copy OpenAL32.lib to `tec/lib`.
		4. Copy soft_oal.dll to `tec/build/bin/Debug|Release` after you build tec.
	4. Build everything (an error about missing bullet libs, for `TEC`, is ok at this point).
    5. Return to cmake-gui and rerun configure and generate so Bullet can now find the libraries you just built.
    6. In the project properties for `TEC` change the `Debugging`->`Working Directory` to `$(SolutionDir)..\`.
	7. In the project properties for `TEC` change the `Linker`->`Advanced`->`Image Has Safe Exception Hanlding` to `No (/SAFESEH:NO)`. (Needed for OpenAL being built with an older Visual Studio)
5. Run it from `tec/`
