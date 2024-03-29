cmake_minimum_required(VERSION 3.19)
project(trillek VERSION 0.1)

option(BUILD_CLIENT "Build the client" ON)
option(BUILD_SERVER "Build the server" ON)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)

set(BUILD_STATIC_VCOMPUTER TRUE CACHE BOOL "Build Trillek VCOMPUTER library - static version")
set(BUILD_DYNAMIC_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER library - dynamic version")
set(BUILD_TOOLS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tools")
set(BUILD_TESTS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tests")

include(cmake/helpers.cmake)
include(cmake/link_helpers.cmake)

add_subdirectory(deps/trillek-vcomputer ${trillek_BINARY_DIR}/deps/trillek-vcomputer)

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
endif()
