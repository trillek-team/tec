cmake_minimum_required(VERSION 3.9)
project(trillek-common)

set(trillek-proto-INCLUDE_DIR ${trillek_BINARY_DIR}/common PARENT_SCOPE) # <-- Protobuf files go here

find_package(protobuf CONFIG REQUIRED)
find_package(Protobuf REQUIRED) # For protobuf_generate_cpp
find_package(Bullet REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)

include_directories(SYSTEM ${trillek_BINARY_DIR}/common)
include_directories(SYSTEM ${BULLET_INCLUDE_DIR})
include_directories(SYSTEM ${trillek_SOURCE_DIR}/deps/selene/include)
include_directories(SYSTEM ${trillek_SOURCE_DIR}/deps/trillek-vcomputer/include)

protobuf_generate_cpp(COMPONENTS_SOURCES COMPONENTS_HEADERS ${trillek-common_SOURCE_DIR}/proto/components.proto)
protobuf_generate_cpp(GAME_STATE_SOURCES GAME_STATE_HEADERS ${trillek-common_SOURCE_DIR}/proto/game_state.proto)
protobuf_generate_cpp(SAVE_GAME_SOURCES SAVE_GAME_HEADERS ${trillek-common_SOURCE_DIR}/proto/save_game.proto)
protobuf_generate_cpp(COMMANDS_SOURCES COMMANDS_HEADERS ${trillek-common_SOURCE_DIR}/proto/commands.proto)

set(trillek-common_LIBRARY "tec" CACHE STRING "Name of the library with code common to both the client and the server")

set(trillek-common_SOURCES
	${trillek-common_SOURCE_DIR}/filesystem.cpp
	${trillek-common_SOURCE_DIR}/lua-system.cpp
	${trillek-common_SOURCE_DIR}/net-message.cpp
	${trillek-common_SOURCE_DIR}/physics-system.cpp
	${trillek-common_SOURCE_DIR}/save-game.cpp
	${trillek-common_SOURCE_DIR}/simulation.cpp
	${trillek-common_SOURCE_DIR}/string.cpp
	${trillek-common_SOURCE_DIR}/tec-types.cpp
	${trillek-common_SOURCE_DIR}/vcomputer-system.cpp
	${trillek-common_SOURCE_DIR}/components/collision-body.cpp
	${trillek-common_SOURCE_DIR}/components/lua-script.cpp
	${trillek-common_SOURCE_DIR}/components/transforms.cpp
	${trillek-common_SOURCE_DIR}/controllers/fps-controller.cpp
	${trillek-common_SOURCE_DIR}/resources/mesh.cpp
	${trillek-common_SOURCE_DIR}/resources/script-file.cpp
)

# Used by IDEs to nicely view header files, dont GLOB cpp though!
file(GLOB_RECURSE trillek-common_HEADERS LIST_DIRECTORIES false ${trillek-common_SOURCE_DIR}/*.hpp)
source_group(Headers FILES ${trillek-common_HEADERS})
set_property(SOURCE ${trillek-common_HEADERS} PROPERTY HEADER_FILE_ONLY true)
file(GLOB trillek-common_PROTOCOLS LIST_DIRECTORIES false ${trillek-common_SOURCE_DIR}/proto/*.proto)
source_group(Proto FILES ${trillek-common_PROTOCOLS})
set_property(SOURCE ${trillek-common_PROTOCOLS} PROPERTY HEADER_FILE_ONLY true)

add_library(${trillek-common_LIBRARY} STATIC
	${COMPONENTS_SOURCES}
	${GAME_STATE_SOURCES}
	${COMMANDS_SOURCES}
	${SAVE_GAME_SOURCES}
	${trillek-common_SOURCES}
	${trillek-common_HEADERS}
	${trillek-common_PROTOCOLS}
)

if (MSVC)
	set_property(TARGET ${trillek-common_LIBRARY} APPEND_STRING PROPERTY COMPILE_FLAGS /W4)
else ()
	set_property(TARGET ${trillek-common_LIBRARY} APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endif ()

target_link_libraries(${trillek-common_LIBRARY} ${BULLET_LIBRARIES} ${LUA_LIBRARIES} sol2::sol2
	protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite spdlog::spdlog VCOMPUTER_STATIC)
