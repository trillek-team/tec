cmake_minimum_required(VERSION 3.9)
project(trillek-client)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

include_directories(${trillek_SOURCE_DIR})
include_directories(SYSTEM ${BULLET_INCLUDE_DIR})
include_directories(SYSTEM ${trillek-proto-INCLUDE_DIR})
include_directories(SYSTEM ${trillek_SOURCE_DIR}/deps/selene/include)
include_directories(SYSTEM ${trillek_SOURCE_DIR}/deps/trillek-vcomputer/include)
include_directories(${trillek-client_SOURCE_DIR})

if (WIN32)
	set(trillek-client_PROGRAM "trillek-client")
else ()
	set(trillek-client_PROGRAM "trillek-client")
endif ()

set(trillek-client_SOURCES # don't include main.cpp to keep it out of tests
	${trillek-client_SOURCE_DIR}/game.cpp
	${trillek-client_SOURCE_DIR}/imgui-system.cpp
	${trillek-client_SOURCE_DIR}/client-game-state-queue.cpp
	${trillek-client_SOURCE_DIR}/os.cpp
	${trillek-client_SOURCE_DIR}/render-system.cpp
	${trillek-client_SOURCE_DIR}/server-connection.cpp
	${trillek-client_SOURCE_DIR}/sound-system.cpp
	${trillek-client_SOURCE_DIR}/test-data.cpp
	${trillek-client_SOURCE_DIR}/voxel-volume.cpp
	${trillek-client_SOURCE_DIR}/graphics/animation.cpp
	${trillek-client_SOURCE_DIR}/graphics/gbuffer.cpp
	${trillek-client_SOURCE_DIR}/graphics/material.cpp
	${trillek-client_SOURCE_DIR}/graphics/renderable.cpp
	${trillek-client_SOURCE_DIR}/graphics/shader.cpp
	${trillek-client_SOURCE_DIR}/graphics/texture-object.cpp
	${trillek-client_SOURCE_DIR}/graphics/vertex-buffer-object.cpp
	${trillek-client_SOURCE_DIR}/gui/console.cpp
	${trillek-client_SOURCE_DIR}/gui/active-entity-tooltip.cpp
	${trillek-client_SOURCE_DIR}/gui/server-connect.cpp
	${trillek-client_SOURCE_DIR}/gui/debug-info.cpp
	${trillek-client_SOURCE_DIR}/resources/md5anim.cpp
	${trillek-client_SOURCE_DIR}/resources/md5mesh.cpp
	${trillek-client_SOURCE_DIR}/resources/obj.cpp
	${trillek-client_SOURCE_DIR}/resources/vorbis-stream.cpp
	${trillek-client_SOURCE_DIR}/resources/pixel-buffer.cpp
)

set(trillek-client_SOURCES ${trillek-client_SOURCES} PARENT_SCOPE) # so tests can use them

# Used by IDEs to nicely view header files
file(GLOB_RECURSE trillek-client_HEADERS LIST_DIRECTORIES false ${trillek-client_SOURCE_DIR}/*.hpp)
source_group(Headers FILES ${trillek-client_HEADERS})
set_property(SOURCE ${trillek-client_HEADERS} PROPERTY HEADER_FILE_ONLY true)

# macOS needs to link libobjc
set(OSX_LIBRARIES "")

IF (APPLE)
	EXEC_PROGRAM(uname ARGS -v OUTPUT_VARIABLE DARWIN_VERSION)
	STRING(REGEX MATCH "[0-9]+" DARWIN_VERSION ${DARWIN_VERSION})

	IF (DARWIN_VERSION LESS 20)
		# No longer needed as of 11.0.1, see issue #179
		set(OSX_LIBRARIES "/usr/lib/libobjc.dylib")
	ENDIF (DARWIN_VERSION LESS 20)

	# Rather wonderfully, Apple decided to mark every single fucking OpenGL function
	# as deprecated because they have 'deprecated' OpenGL. Brilliant!
	add_definitions(-DGL_SILENCE_DEPRECATION)
ENDIF(APPLE)

if (UNIX AND NOT APPLE)
	find_package(X11 REQUIRED)
endif (UNIX AND NOT APPLE)

add_executable(${trillek-client_PROGRAM} ${trillek-client_SOURCE_DIR}/main.cpp ${trillek-client_SOURCES} ${trillek-client_HEADERS})

if (MSVC)
	set_property(TARGET ${trillek-client_PROGRAM} APPEND_STRING PROPERTY COMPILE_FLAGS /W4)
else ()
	set_property(TARGET ${trillek-client_PROGRAM} APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endif ()

target_link_libraries(${trillek-client_PROGRAM} ${trillek-common_LIBRARY}
	${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} ${OSX_LIBRARIES} GLEW::GLEW OpenAL::OpenAL glfw imgui::imgui VCOMPUTER_STATIC)
