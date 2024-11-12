# Trillek Engine C
| Github actions | RTD docs | Code Coverage |
|-------|-------|-------|
|[![Trillek Engine CI](https://github.com/trillek-team/tec/actions/workflows/tec.yml/badge.svg)](https://github.com/trillek-team/tec/actions/workflows/tec.yml)|[![Documentation Status](https://readthedocs.org/projects/project-trillek/badge/?version=latest)](https://project-trillek.readthedocs.io/?badge=latest)|[![codecov](https://codecov.io/gh/trillek-team/tec/branch/master/graph/badge.svg?token=ikZxlqltAW)](https://codecov.io/gh/trillek-team/tec)|

## Support
Head onto our [Discord](https://discord.gg/HM8hhbGSjd) for extended support on building and usage.
![Discord Shield](https://discordapp.com/api/guilds/671106829853523969/widget.png?style=shield)

## Requirements
TEC requires cmake 3.19, a c++17 compatible compiler, and a few libraries GLFW3, GLM, ASIO, Protobuf, GLAD, Lua, Bullet, Dear ImGui, sol3, Spdlog and OpenAL.

The libraries are automatically installed when using CMake and VCPKG [manifest mode](https://vcpkg.readthedocs.io/en/latest/users/manifests/).

# Quick Start
## OS Specific first steps
### Windows
*Potentially* Download and install oalinst.zip [OpenAL installer](http://openal.org/downloads/) and install it.
### MacOS
Prior to 11.0.1, run (NOT TESTED): `sudo installer -pkg /Library/Developer/CommandLineTools/Packages/macOS_SDK_headers_for_macOS_10.14.pkg -target /`

If `bootstrap-vcpkg` fails see [here](https://donatstudios.com/MojaveMissingHeaderFiles) for more help
### Linux
Install the following. This is a pretty extensive list and may be more than needed

`build-essential pkg-config tar curl zip unzip gdb libgl1-mesa-dev xorg-dev libglu1-mesa-dev libxinerama-dev libxcursor-dev`

## Build Steps
1. Install [CMake](https://cmake.org/download/)
2. Install [VCPK](https://github.com/Microsoft/vcpkg.git) - `git clone https://github.com/Microsoft/vcpkg.git`
3. `VCPKG_ROOT` must be set a an environment variable, to wherever VCPKG was cloned to
4. Run `$(VCPKG_ROOT)/bootstrap-vcpkg.[bat|sh]` to setup vcpkg. Use the correct extension for your OS.
   1. [OPTIONAL] `$(VCPKG_ROOT)/./vcpkg integrate install`
5. Open the folder in an editor OR see step 6
   1. [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-presets-vs?view=msvc-170)
   2. [CLion](https://www.jetbrains.com/help/clion/cmake-presets.html)
6. Run CMake
   1. CLI
      1. `cmake --preset=ninja-multi-vcpkg`
      2. `cmake --build --preset=BUILD_PRESET` see [presets](#presets)
   2. GUI - This will only configure the project. CLI steps or Open in an editor are more useful.
      1. Set the source folder
      2. Set the build folder. The presets default to `builds/CONFIG_NAME` e.g. `builds/msvc/`
      3. Run Configure
         1. Pick the Ninja Multi-config generator
         2. Select Specify toolchain file for cross compiling
         3. The file input should be filled in, but if not point it to `$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake`
      4. Run Generate
      5. Open generated solution and build the project(s)

# In depth
## Build Tool Dependencies:
* [cmake](https://cmake.org/)
* [vcpkg](https://github.com/microsoft/vcpkg)
* [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages) ***Optional***

## Linux / WSL Dependencies
* `gcc10 g++10` or `clang-13`
* `build-essential pkg-config tar curl zip unzip gdb`
* `libgl1-mesa-dev xorg-dev libglu1-mesa-dev libxinerama-dev libxcursor-dev p7zip-full`

## Supported editor(s)
* Visual Studio
* Visual Studio (CMake folder mode)
  * Change Tools -> Options -> CMake -> General and enable "Prefer using CMake Presets..."
* Visual Studio Code
  * On Windows you may need to add the CMake bin to the system path, for CTest to work.
* CLion
  * Not tested, but CMake folder mode should work as expected

# Presets
## Config:
* `ninja-multi-vcpkg` - Ninja multi-build config preset that references the vcpkg toolchain file relative to the `VCPKG_ROOT`

## Build:
* `ninja-multi-vcpkg-debug` - Debug configuration build preset that uses the `ninja-multi-vcpkg` config
* `ninja-multi-vcpkg-release-dbginfo` - Release with debug info configuration build preset that uses the `ninja-multi-vcpkg` config
* `ninja-multi-vcpkg-release` - Release configuration build preset that uses the `ninja-multi-vcpkg` config
* `ninja-multi-docs` - Doc build preset with the documentation custom [targets](#documentation-targets)

## Test
* `test-base` - Base test preset that all other tests derive from **DO NOT USE**
* `ninja-multi-vcpkg-debug` - Debug configuration test preset
* `ninja-multi-vcpkg-release` - Release configuration test preset

# Documentation
Documentation is done via Doxygen for C++ code and supplementary docs must be maintained for the Lua API in `docs/`.

Engine documentation is generated with the with the `ninja-multi-docs` build config `doxygen` target, and is then
converted to markdown via `doxybook2` with the `ninja-multi-docs` build config `doxybook` target.

All documentation is converted to mkdocs format with the `ninja-multi-docs` build config `mkdocs` target, and then
readthedocs hosts the mkdocs files.

Doxygen and mkdocs files **are not** committed, but can be generated locally for consumption/validation. Doxybook
generated engine docs and all other docs in `docs/` **are** committed.

## Documentation Dependencies
* [doxygen](https://www.doxygen.nl/) 1.9.4
* [doxybook2](https://github.com/matusnovak/doxybook2) 1.4.0
* [python](https://www.python.org/) 3.10.4 (Earlier version may work, but untested)
* [pip](https://pip.pypa.io/en/stable/) 22.0.4 (Earlier version may work, but untested)
* [pip-tools](https://github.com/jazzband/pip-tools) 6.8.0 (Earlier version may work, but untested)
* [mkdocs](https://www.mkdocs.org/) 1.3.0 (see docs/requirements)

## Readthedocs setup
To generate the python requirements for mkdocs to be used by readthedoc run `pip-compile docs/requirements.in` in the project root.

## Documentation Targets
The following targets can be built when the CMake build preset is set to `ninja-multi-docs`

* doxygen - Generates full doxygen docs in `doxygen/`
* doxybook - Generates markdown versions of doxygen docs and places them in `docs/engine/`
* mkdocs - Generates mkdocs from `docs/` in places them `mkdocs/`

## Local Preview
To preview the mkdocs docs locally visit run `mkdocs serve`, in the project root.

# Clang Format
The follow docker script will setup a docker container that will run clang format.
`docker build -t clang-format-lint github.com/DoozyX/clang-format-lint-action`
## Windows
Run the following on windows to format all source files in the src dir
`docker run -it --rm --workdir /src -v ${pwd}:/src clang-format-lint --clang-format-executable /clang-format/clang-format11 -r -i true .`
## Linux
Run the following on windows to format all source files in the src dir
`docker run -it --rm --workdir /src -v $(pwd):/src clang-format-lint --clang-format-executable /clang-format/clang-format11 -r -i true .`
