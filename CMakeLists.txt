# Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
# Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt
cmake_minimum_required(VERSION 3.9)
project(trillek)

option(BUILD_CLIENT "Build the client" ON)
option(BUILD_SERVER "Build the server" ON)
option(BUILD_TESTS "Build tests" OFF)

# TEMPRARY WORKAROUND - Figure out a better solution (TODO)
#	asio does not include a CMake module so we have no easy way to locate it.
#	As a workaround, include the MacPorts and Fink 'includes' directories.
if (APPLE)
	include_directories("/opt/local/include")
	include_directories("/sw/include")
endif (APPLE)

# TODO: Perhaps add -pedantic -Wall -Wextra -Werror?
list(APPEND CMAKE_MODULE_PATH ${trillek_SOURCE_DIR}/deps)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${trillek_SOURCE_DIR}/common)

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(-DASIO_STANDALONE)

if (MSVC)
	add_definitions(-DPROTOBUF_USE_DLLS)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS )
	add_definitions(-D_WIN32_WINNT=0x0601)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") # Auto generated protobuf gives lots of these and they recommend to disable
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127") # Warns that expression could be 'if constexpr' which 3rd party libs might not be updated to use
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4005") # Protobuf redefines the macro by mistake
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4201") # GLM uses language extensions if it can
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4125") # Auto generated protobuf gives lots of these on msvc
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus") # MSVC 
endif ()

set(BUILD_STATIC_VCOMPUTER TRUE CACHE BOOL "Build Trillek VCOMPUTER library - static version")
set(BUILD_DYNAMIC_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER library - dynamic version")
set(BUILD_TOOLS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tools")
set(BUILD_TESTS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tests")
add_subdirectory(deps/trillek-vcomputer ${trillek_BINARY_DIR}/deps/trillek-vcomputer)

add_subdirectory(common)
if (BUILD_CLIENT)
	add_subdirectory(client)
endif ()
if (BUILD_SERVER)
	add_subdirectory(server)
endif ()
if (BUILD_TESTS)
	add_subdirectory(tests)
endif ()
