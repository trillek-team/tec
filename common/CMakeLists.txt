cmake_minimum_required(VERSION 3.19)
project(trillek-common)

find_package(Protobuf CONFIG REQUIRED)

set(trillek-common_PROTOCOLS
	${CMAKE_CURRENT_SOURCE_DIR}/proto/commands.proto
	${CMAKE_CURRENT_SOURCE_DIR}/proto/components.proto
	${CMAKE_CURRENT_SOURCE_DIR}/proto/game_state.proto
	${CMAKE_CURRENT_SOURCE_DIR}/proto/graphics.proto
	${CMAKE_CURRENT_SOURCE_DIR}/proto/save_game.proto
)

set_source_group_property(FILE_LIST ${trillek-common_PROTOCOLS} GROUP_NAME Proto)
protobuf_generate_cpp(trillek-common_GENSOURCES trillek-common_GENHEADERS ${trillek-common_PROTOCOLS})

set(trillek-common_LIBRARY_NAME "tec")
set(trillek-common_LIBRARY_NAME ${trillek-common_LIBRARY_NAME} PARENT_SCOPE)
add_library(${trillek-common_LIBRARY_NAME} STATIC ${trillek-common_GENSOURCES} ${trillek-common_PROTOCOLS})
link_common_libs(TARGET ${trillek-common_LIBRARY_NAME})
set_warning_flag(TARGET ${trillek-common_LIBRARY_NAME})

file(GLOB_RECURSE trillek-common_HEADERS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

target_include_directories(
	${trillek-common_LIBRARY_NAME}
	PUBLIC 
		${CMAKE_CURRENT_BINARY_DIR}
		${trillek_SOURCE_DIR}/deps/trillek-vcomputer/include)
target_compile_features(${trillek-common_LIBRARY_NAME} PUBLIC cxx_std_17)
target_link_libraries(${trillek-common_LIBRARY_NAME} PUBLIC VCOMPUTER_STATIC)
target_sources(${trillek-common_LIBRARY_NAME}
	PUBLIC
		file-factories.cpp
		filesystem.cpp
		filesystem_platform.cpp
		lua-system.cpp
		net-message.cpp
		physics-system.cpp
		proto-load.cpp
		simulation.cpp
		string.cpp
		tec-types.cpp
		vcomputer-system.cpp
		components/collision-body.cpp
		components/lua-script.cpp
		components/transforms.cpp
		controllers/fps-controller.cpp
		resources/mesh.cpp
		resources/script-file.cpp
	PUBLIC FILE_SET HEADERS FILES ${trillek-common_HEADERS})