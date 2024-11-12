function (setup_compiler)
	set(isMSVC "$<CXX_COMPILER_ID:MSVC>")
	set(isCLANG "$<CXX_COMPILER_ID:Clang>")
	set(isWin32 "$<PLATFORM_ID:Windows>")

	set(msvcFlags /W4 /experimental:external /external:anglebrackets /external:W0)
	add_compile_options("$<IF:${isMSVC},${msvcFlags},-Wall>")

	set(msvcDefines
		"PROTOBUF_USE_DLLS;_CRT_SECURE_NO_WARNINGS;_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;_WIN32_WINNT=0x0601;WIN32_LEAN_AND_MEAN"
	)
	add_compile_definitions("$<${isMSVC}:${msvcDefines}>" "$<${isWin32}:WIN32>")
endfunction ()

# Used by IDEs to nicely view header files
function (set_source_group_property)
	cmake_parse_arguments("SET_SOURCE_GROUP_PROPERTY" "" "GROUP_NAME" "FILE_LIST" ${ARGN})

	if (SET_SOURCE_GROUP_PROPERTY_GROUP_NAME)
		set(GROUP_NAME ${SET_SOURCE_GROUP_PROPERTY_GROUP_NAME})
	else ()
		set(GROUP_NAME Headers)
	endif ()

	if (SET_SOURCE_GROUP_PROPERTY_FILE_LIST)
		source_group(
			TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX ${GROUP_NAME}
			FILES ${SET_SOURCE_GROUP_PROPERTY_FILE_LIST}
		)
		set_property(SOURCE ${SET_SOURCE_GROUP_PROPERTY_FILE_LIST} PROPERTY HEADER_FILE_ONLY true)
	endif ()
endfunction ()

# Consistent add_executable
function (add_program)
	cmake_parse_arguments("ADD_PROGRAM" "" "TARGET" "FILE_LIST;LINK_LIBS" ${ARGN})

	add_executable(${ADD_PROGRAM_TARGET} ${ADD_PROGRAM_FILE_LIST})

  target_include_directories(${ADD_PROGRAM_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
	target_link_libraries(${ADD_PROGRAM_TARGET} PUBLIC ${COMMON_LIB_NAME} ${ADD_PROGRAM_LINK_LIBS})
	target_compile_features(${ADD_PROGRAM_TARGET} PUBLIC cxx_std_17)

	if (MSVC)
		set_property(TARGET ${ADD_PROGRAM_TARGET} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
	endif ()
endfunction ()

function (add_interface_lib)
	cmake_parse_arguments("ADD_INTERFACE_LIB" "" "TARGET" "FILE_LIST" ${ARGN})
	add_library(${ADD_INTERFACE_LIB_TARGET} INTERFACE)
	file(GLOB_RECURSE CURRENT_SOURCE_DIR_HEADERS LIST_DIRECTORIES false ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
	target_sources(${ADD_INTERFACE_LIB_TARGET} PUBLIC ${ADD_INTERFACE_LIB_FILE_LIST} ${CURRENT_SOURCE_DIR_HEADERS})
	target_include_directories(${ADD_INTERFACE_LIB_TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
	target_compile_features(${ADD_INTERFACE_LIB_TARGET} INTERFACE cxx_std_17)
endfunction ()
