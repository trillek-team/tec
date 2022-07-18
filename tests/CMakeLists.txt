cmake_minimum_required(VERSION 3.19)

enable_testing()
include(Dart)
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)
find_package(protobuf CONFIG REQUIRED)
find_package(Bullet REQUIRED)
find_package(Lua REQUIRED)
find_package(spdlog CONFIG REQUIRED)
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
include_directories(${trillek-server_SOURCE_DIR})

set(trillek-test_PROGRAM "tests")

set(trillek-test_SOURCES
	filesystem_test.cpp
	net-message_test.cpp
	save-game_test.cpp
	server-client-connection.cpp
	user_test.cpp
)

source_group(Tests FILES ${trillek-test_SOURCES})

add_executable(${trillek-test_PROGRAM}
	${trillek-test_SOURCES}
	${trillek-server_SOURCES}
	${trillek-client_SOURCES}
)
target_link_libraries(${trillek-test_PROGRAM} PRIVATE ${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} ${OSX_LIBRARIES}
	GTest::gtest GTest::gtest_main GLEW::GLEW OpenAL::OpenAL glfw imgui::imgui ${trillek-common_LIBRARY} VCOMPUTER_STATIC)
gtest_discover_tests(${trillek-test_PROGRAM} WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
