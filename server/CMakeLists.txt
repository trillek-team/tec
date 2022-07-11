cmake_minimum_required(VERSION 3.19)
project(trillek-server)

include_directories(${trillek_SOURCE_DIR})
include_directories(SYSTEM ${BULLET_INCLUDE_DIR})
include_directories(SYSTEM ${trillek-proto-INCLUDE_DIR})
include_directories(SYSTEM ${trillek_SOURCE_DIR}/deps/selene/include)
include_directories(SYSTEM ${trillek_SOURCE_DIR}/deps/trillek-vcomputer/include)
include_directories(${trillek-server_SOURCE_DIR})

if (WIN32)
	set(trillek-server_PROGRAM "trillek-server")
else ()
	set(trillek-server_PROGRAM "trillek-serverd")
endif ()

set(trillek-server_SOURCES # don't include main.cpp to keep it out of tests
	${trillek-server_SOURCE_DIR}/client-connection.cpp
	${trillek-server_SOURCE_DIR}/lua-types.cpp
	${trillek-server_SOURCE_DIR}/save-game.cpp
	${trillek-server_SOURCE_DIR}/server.cpp
	${trillek-server_SOURCE_DIR}/server-game-state-queue.cpp
	${trillek-server_SOURCE_DIR}/user/user.cpp
)

set(trillek-server_SOURCES ${trillek-server_SOURCES} PARENT_SCOPE) # so tests can use them

# Used by IDEs to nicely view header files, don't depend on this to work!
file(GLOB_RECURSE trillek-server_HEADERS LIST_DIRECTORIES false ${trillek-server_SOURCE_DIR}/*.hpp)
source_group(Headers FILES ${trillek-server_HEADERS})
set_property(SOURCE ${trillek-server_HEADERS} PROPERTY HEADER_FILE_ONLY true)

add_executable(${trillek-server_PROGRAM}
	${trillek-server_SOURCE_DIR}/main.cpp
	${trillek-server_SOURCES}
	${trillek-server_HEADERS}
)

if (MSVC)
	set_property(TARGET ${trillek-server_PROGRAM} APPEND_STRING PROPERTY COMPILE_FLAGS /W4)
else ()
	set_property(TARGET ${trillek-server_PROGRAM} APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endif ()

target_link_libraries(${trillek-server_PROGRAM} ${trillek-common_LIBRARY} VCOMPUTER_STATIC)
