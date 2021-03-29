# Trillek Engine C
| Windows (VStudio 2019)  | Semaphore (GCC) |
|-------------------------|-----------------|
| [![Build status](https://ci.appveyor.com/api/projects/status/809xi9ukwo7sgsip?svg=true)](https://ci.appveyor.com/project/adam4813/tec-hem9u) | [![Build Status](https://semaphoreci.com/api/v1/trillek-team/tec/branches/master/shields_badge.svg)](https://semaphoreci.com/trillek-team/tec) |

## Support
Head onto our [Discord](https://discord.gg/HM8hhbGSjd) for extended support on building and usage.  
![Discord Shield](https://discordapp.com/api/guilds/671106829853523969/widget.png?style=shield)

## Requirements
TEC requires cmake 3.9 and a few libraries GLFW3, GLM, ASIO, Protobuf, GLEW, Lua, Bullet, Dear ImGui, sol3, Spdlog and OpenAL which can be installed most easily via [vcpkg](#vcpkg)

## Documentation
Read the docs on RTFD
[![Documentation Status](https://readthedocs.org/projects/project-trillek/badge/?version=latest)](https://project-trillek.readthedocs.io/?badge=latest) 
Documentation is done via Doxygen for C++ code and supplementary docs must be maintained for the Lua API in the `docs/` folder.

Whenever there is a Lua API change please update the corresponding `docs/`.

### Local Preview
To preview the Lua API docs locally visit [MkDocs](https://www.mkdocs.org/) and follow their setup guide. Then from the root directory run `mkdocs serve`.

## Building from Source
### Part 1 (VCPKG/Requirements)
1. `git clone https://github.com/Microsoft/vcpkg.git` (If you already have **VCPKG**, move onto Part 2))
1. Navigate to the `vcpkg/` directory.

#### Windows
	./bootstrap-vcpkg.bat
	[OPTIONAL] ./vcpkg integrate install
	./vcpkg install asio bullet3 glew glfw3 glm lua openal-soft protobuf zlib spdlog imgui sol2

#### MacOS
Prior to 11.0.1, run (NOT TESTED): `sudo installer -pkg /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg -target /` if `bootstrap-vcpkg` fails see [here](https://donatstudios.com/MojaveMissingHeaderFiles) for more help

	./bootstrap-vcpkg.sh
	[OPTIONAL] ./vcpkg integrate install
	./vcpkg install asio bullet3 glew glfw3 glm lua openal-soft protobuf zlib spdlog imgui sol2

#### Linux
	apt-get install libgl1-mesa-dev xorg-dev libglu1-mesa-dev libxinerama-dev libxcursor-dev
	./bootstrap-vcpkg.sh
	[OPTIONAL] ./vcpkg integrate install
	./vcpkg install asio bullet3 glew glfw3 glm lua openal-soft protobuf zlib spdlog imgui sol2

### Part 2 (Building)
1. `git submodule update --init` in the root directory.
1. `mkdir build/` in to root directory
1. `cd build/`
#### Windows (Visual Studio 2019)
   1. Run cmake-gui setting the source line to the root directory and the build line to the build directory.
   1. Hit configure and select `Specify toolchain file for cross-compiling` using `**VCPKG_DIR**/scripts/buildsystems/vcpkg.cmake`.
   1. Click generate; then open and build the solution in Visual Studio.
   1. In the project properties for `trillek-client` change the `Debugging`->`Working Directory` to `$(SolutionDir)..\`.
   1. **Potentially** Download and install oalinst.zip [OpenAL installer](http://openal.org/downloads/) and install it.
#### Linux
##### VCPKG (G++ 7 or CLang 4)
   1. `cmake -DCMAKE_TOOLCHAIN_FILE=**VCPKG_DIR**/scripts/buildsystems/vcpkg.cmake ..` in the build directory
   1. `make` in the build directory
##### NON-VCPKG - G++ 7 or CLang 4 (Unsupported due to cmake)
   1. **INSTALL REQUIRED LIBS** bullet, glew, glfw3, glm, asio, lua, openal-soft, spdlog, Dear ImGui, sol3(also known as sol2), and protobuf. Some of these will need versions not in your distribution (just ask for help in the IRC or Discord.)
      1. If you are on Ubuntu/Debian/etc. (something with `apt`):
         1. Run `apt-get install libglew-dev libglfw3 libglm-dev libasio-dev`
         2. Run `apt-get install liblua5.2-dev libopenal-dev  libbullet-dev`
         3. Run `apt-get install libprotobuf-dev protobuf-compiler libspdlog-dev`
      1. If you are on Arch/etc. (something with `pacman`):
         1. Run `pacman -S glew glfw-x11 glm asio lua52 openal bullet protobuf spdlog`
   1. `cmake ..` in the build directory
   1. `make tec` in the build directory

#### Mac OS
   1. `cmake -DCMAKE_TOOLCHAIN_FILE=**VCPKG_DIR**/scripts/buildsystems/vcpkg.cmake ..` in the build directory
   1. `make` in the build directory

### Part 3 (Unit Tests)
To generate the unit tests, follow the same instructions from before, but set to true the flag BUILD_TESTS_TEC

## Clang Format
The follow docker script will setup a docker container that will run clang format.
`docker build -t clang-format-lint github.com/DoozyX/clang-format-lint-action`
### Windows
Run the following on windows to format all source files in the src dir
`docker run -it --rm --workdir /src -v ${pwd}:/src clang-format-lint --clang-format-executable /clang-format/clang-format11 -r -i true .`
### Linux
Run the following on windows to format all source files in the src dir
`docker run -it --rm --workdir /src -v $(pwd):/src clang-format-lint --clang-format-executable /clang-format/clang-format11 -r -i true .`