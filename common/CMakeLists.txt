cmake_minimum_required(VERSION 3.19)
project(trillek-common)

find_package(Protobuf CONFIG REQUIRED)

set(PROTO_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/proto/commands.proto
	${CMAKE_CURRENT_SOURCE_DIR}/proto/components.proto
	${CMAKE_CURRENT_SOURCE_DIR}/proto/game_state.proto
	${CMAKE_CURRENT_SOURCE_DIR}/proto/graphics.proto
	${CMAKE_CURRENT_SOURCE_DIR}/proto/save_game.proto
)
protobuf_generate_cpp(PROTOGEN_SOURCES PROTOGEN_HEADERS ${PROTO_FILES})

set(COMMON_LIB_NAME "tec")
set(COMMON_LIB_NAME ${COMMON_LIB_NAME} PARENT_SCOPE)
add_library(${COMMON_LIB_NAME} STATIC ${PROTOGEN_SOURCES} ${PROTO_FILES})
target_compile_features(${COMMON_LIB_NAME} PUBLIC cxx_std_17)

target_include_directories(
	${COMMON_LIB_NAME}
	PUBLIC 
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/deps/trillek-vcomputer/include)

find_package(Lua REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(
	${COMMON_LIB_NAME}
	PUBLIC
		VCOMPUTER_STATIC
		${BULLET_LIBRARIES}
		${LUA_LIBRARIES}
		protobuf::libprotobuf
		spdlog::spdlog
		asio::asio
		glm::glm)

file(GLOB_RECURSE COMMON_HEADERS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
target_sources(
	${COMMON_LIB_NAME}
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
		${COMMON_HEADERS})

set_source_group_property(FILE_LIST ${PROTO_FILES} GROUP_NAME Proto)
