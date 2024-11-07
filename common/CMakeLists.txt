cmake_minimum_required(VERSION 3.20)
project(trillek-common)

find_package(Protobuf CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

set(PROTO_FILES proto/commands.proto proto/components.proto proto/game_state.proto proto/graphics.proto
								proto/save_game.proto
)
protobuf_generate_cpp(PROTOGEN_SOURCES PROTOGEN_HEADERS ${PROTO_FILES})

set(COMMON_LIB_NAME "tec")
set(COMMON_LIB_NAME ${COMMON_LIB_NAME} PARENT_SCOPE)

add_library(${COMMON_LIB_NAME} STATIC)

target_link_libraries(
	${COMMON_LIB_NAME}
	PUBLIC BulletSoftBody
				 BulletDynamics
				 BulletCollision
				 Bullet3Common
				 LinearMath
				 ${LUA_LIBRARIES}
				 protobuf::libprotobuf
				 spdlog::spdlog
				 asio::asio
				 glm::glm
				 VCOMPUTER_STATIC
)

target_compile_definitions(${COMMON_LIB_NAME} PUBLIC GLM_ENABLE_EXPERIMENTAL)

target_sources(
	${COMMON_LIB_NAME}
	PUBLIC ${PROTOGEN_SOURCES}
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
	PUBLIC ${PROTO_FILES}
)

target_include_directories(
	${COMMON_LIB_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
														${CMAKE_SOURCE_DIR}/deps/trillek-vcomputer/include # Add generated headers path
)
set_source_group_property(FILE_LIST ${PROTO_FILES} GROUP_NAME Proto)
