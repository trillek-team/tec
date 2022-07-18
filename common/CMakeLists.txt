cmake_minimum_required(VERSION 3.19)
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

set(trillek-common_PROTOCOLS
	${trillek-common_SOURCE_DIR}/proto/commands.proto
	${trillek-common_SOURCE_DIR}/proto/components.proto
	${trillek-common_SOURCE_DIR}/proto/game_state.proto
	${trillek-common_SOURCE_DIR}/proto/graphics.proto
	${trillek-common_SOURCE_DIR}/proto/save_game.proto
)

foreach(PROTO_FILE ${trillek-common_PROTOCOLS})
	protobuf_generate_cpp(PROTOGENCPP trillek-common_GENHEADERS ${PROTO_FILE})
	list(APPEND trillek-common_GENSOURCES ${PROTOGENCPP})
endforeach()

set(trillek-common_LIBRARY "tec" CACHE STRING "Name of the library with code common to both the client and the server")

set(trillek-common_SOURCES
	${trillek-common_SOURCE_DIR}/file-factories.cpp
	${trillek-common_SOURCE_DIR}/filesystem.cpp
	${trillek-common_SOURCE_DIR}/filesystem_platform.cpp
	${trillek-common_SOURCE_DIR}/lua-system.cpp
	${trillek-common_SOURCE_DIR}/net-message.cpp
	${trillek-common_SOURCE_DIR}/physics-system.cpp
	${trillek-common_SOURCE_DIR}/proto-load.cpp
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
source_group(Proto FILES ${trillek-common_PROTOCOLS})
set_property(SOURCE ${trillek-common_PROTOCOLS} PROPERTY HEADER_FILE_ONLY true)

add_library(${trillek-common_LIBRARY} STATIC
	${trillek-common_GENSOURCES}
	${trillek-common_SOURCES}
	${trillek-common_HEADERS}
	${trillek-common_PROTOCOLS}
)

if (MSVC)
	set_property(TARGET ${trillek-common_LIBRARY} APPEND_STRING PROPERTY COMPILE_FLAGS /W4)
else ()
	set_property(TARGET ${trillek-common_LIBRARY} APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endif ()

target_compile_features(${trillek-common_LIBRARY} PUBLIC cxx_std_17)

target_link_libraries(${trillek-common_LIBRARY} ${BULLET_LIBRARIES} ${LUA_LIBRARIES} sol2
	protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite spdlog::spdlog VCOMPUTER_STATIC)
