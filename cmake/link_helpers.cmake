function (link_opengl_libs)
	cmake_parse_arguments("LINK_OPENGL_LIBS" "" "TARGET" "" ${ARGN})
	if (TARGET ${LINK_OPENGL_LIBS_TARGET})
		message("Linking OpenGL libs to ${LINK_OPENGL_LIBS_TARGET}")
		find_package(OpenGL REQUIRED)
		if (APPLE)
			exec_program(uname ARGS -v OUTPUT_VARIABLE DARWIN_VERSION)
			string(REGEX MATCH "[0-9]+" DARWIN_VERSION ${DARWIN_VERSION})

			if (DARWIN_VERSION LESS 20)
				# No longer needed as of 11.0.1, see issue #179
				set(OSX_LIBRARIES "/usr/lib/libobjc.dylib")
			endif (DARWIN_VERSION LESS 20)

			# Rather wonderfully, Apple decided to mark every single fucking OpenGL function as deprecated because they have
			# 'deprecated' OpenGL. Brilliant!
			add_definitions(-DGL_SILENCE_DEPRECATION)
		elseif (UNIX AND NOT APPLE)
			find_package(X11 REQUIRED)
		endif (APPLE)

		set(GL_LIBS ${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} ${OSX_LIBRARIES})

		target_link_libraries(${LINK_OPENGL_LIBS_TARGET} INTERFACE ${GL_LIBS})
	endif ()
endfunction ()
