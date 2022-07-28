function(link_client_libs)
	cmake_parse_arguments("LINK_CLIENT_LIBS" "" "TARGET" "" ${ARGN})
	if (TARGET ${LINK_CLIENT_LIBS_TARGET})
		message("Linking client libs to ${LINK_CLIENT_LIBS_TARGET}")

		find_package(OpenGL REQUIRED)
		find_package(GLEW REQUIRED)
		find_package(glfw3 CONFIG REQUIRED)
		find_package(OpenAL CONFIG REQUIRED)
		find_package(imgui CONFIG REQUIRED)

		if (APPLE)
			EXEC_PROGRAM(uname ARGS -v OUTPUT_VARIABLE DARWIN_VERSION)
			STRING(REGEX MATCH "[0-9]+" DARWIN_VERSION ${DARWIN_VERSION})

			IF (DARWIN_VERSION LESS 20)
				# No longer needed as of 11.0.1, see issue #179
				set(OSX_LIBRARIES "/usr/lib/libobjc.dylib")
			ENDIF (DARWIN_VERSION LESS 20)

			# Rather wonderfully, Apple decided to mark every single fucking OpenGL function
			# as deprecated because they have 'deprecated' OpenGL. Brilliant!
			add_definitions(-DGL_SILENCE_DEPRECATION)
		elseif (UNIX AND NOT APPLE)
			find_package(X11 REQUIRED)
		endif (APPLE)

		set(ADDITIONAL_GL_LIBS ${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} ${OSX_LIBRARIES})
		set(CLIENT_LINK_LIBS glfw ${ADDITIONAL_GL_LIBS} GLEW::GLEW OpenAL::OpenAL imgui::imgui)

		target_link_libraries(${LINK_CLIENT_LIBS_TARGET} INTERFACE ${CLIENT_LINK_LIBS})
	endif()
endfunction()

function(link_common_libs)
	cmake_parse_arguments("LINK_COMMON_LIBS" "" "TARGET" "" ${ARGN})
	if (TARGET ${LINK_COMMON_LIBS_TARGET})
		message("Linking common libs to ${LINK_COMMON_LIBS_TARGET}")
		find_package(Lua REQUIRED)
		find_package(Bullet CONFIG REQUIRED)
		find_package(spdlog CONFIG REQUIRED)
		find_package(sol2 CONFIG REQUIRED)
		find_package(asio CONFIG REQUIRED)
		find_package(glm CONFIG REQUIRED)

		add_definitions(-DASIO_STANDALONE)

		set(COMMON_LINK_LIBS ${BULLET_LIBRARIES} ${LUA_LIBRARIES} protobuf::libprotobuf spdlog::spdlog asio::asio glm::glm)
		target_link_libraries(${LINK_COMMON_LIBS_TARGET} PUBLIC ${COMMON_LINK_LIBS})
		target_include_directories(${LINK_COMMON_LIBS_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	endif()
endfunction()

