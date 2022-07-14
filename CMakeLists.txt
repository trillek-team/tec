cmake_minimum_required(VERSION 3.19)
project(trillek)

option(BUILD_CLIENT "Build the client" ON)
option(BUILD_SERVER "Build the server" ON)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# TODO: Perhaps add -pedantic -Wall -Wextra -Werror?
list(APPEND CMAKE_MODULE_PATH ${trillek_SOURCE_DIR}/deps)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DASIO_STANDALONE)

if (WIN32)
	add_definitions(-DWIN32)
endif()

if (MSVC)
	add_definitions(-DPROTOBUF_USE_DLLS -D_CRT_SECURE_NO_WARNINGS -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -D_WIN32_WINNT=0x0601 -DWIN32_LEAN_AND_MEAN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4125") # Auto generated protobuf gives lots of these on msvc
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus") # MSVC define __cplusplus workaround

	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(/EHsc)
	endif()
	# Only MSVC compiler understands these flags
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		# The following turns off warnings in external libraries until the generator respects include_directories(SYSTEM ...) see https://devblogs.microsoft.com/cppblog/broken-warnings-theory/
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /experimental:external /external:anglebrackets /external:W0")
	endif()
endif()

set(BUILD_STATIC_VCOMPUTER TRUE CACHE BOOL "Build Trillek VCOMPUTER library - static version")
set(BUILD_DYNAMIC_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER library - dynamic version")
set(BUILD_TOOLS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tools")
set(BUILD_TESTS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tests")

include_directories(${trillek_SOURCE_DIR}/common)

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
if (BUILD_DOCS)
	add_subdirectory(docs_targets)
endif()
