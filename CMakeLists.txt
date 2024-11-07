cmake_minimum_required(VERSION 3.20)
project(trillek VERSION 0.1 DESCRIPTION "Trillek Game Engine" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_CLIENT "Build the client" ON)
option(BUILD_SERVER "Build the server" ON)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)

set(BUILD_STATIC_VCOMPUTER ON CACHE BOOL "Build Trillek VCOMPUTER library - static version")
set(BUILD_DYNAMIC_VCOMPUTER OFF CACHE BOOL "Build Trillek VCOMPUTER library - dynamic version")
set(BUILD_TOOLS_VCOMPUTER OFF CACHE BOOL "Build Trillek VCOMPUTER tools")
set(BUILD_TESTS_VCOMPUTER OFF CACHE BOOL "Build Trillek VCOMPUTER tests")

include(cmake/helpers.cmake)
include(cmake/link_helpers.cmake)

add_subdirectory(deps/trillek-vcomputer ${PROJECT_BINARY_DIR}/deps/trillek-vcomputer)

setup_compiler()

add_subdirectory(common)
if (BUILD_CLIENT)
	add_subdirectory(client)
endif ()
if (BUILD_SERVER)
	add_subdirectory(server)
endif ()
if (BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif ()
if (BUILD_DOCS)
	add_subdirectory(docs_targets)
endif ()
