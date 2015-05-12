# check for gcc version to set c++11 or c++0x.
# thanks to http://stackoverflow.com/questions/10984442/how-to-detect-c11-support-of-a-compiler-with-cmake .
IF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	IF (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		MESSAGE("Supported GCC!")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	ENDIF (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") # Clang 3.3 and up support c++11.
	MESSAGE("Clang Version: " ${CMAKE_CXX_COMPILER_VERSION})
	# On OS X, Clang 3.3 would be Clang/LLVM 5.0.
	IF (APPLE)
		SET(CLANG_MIN_VERSION 5.0)
	ELSE (APPLE)
		SET(CLANG_MIN_VERSION 3.3)
	ENDIF (APPLE)
	IF (NOT (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER CLANG_MIN_VERSION OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL CLANG_MIN_VERSION))
		# clang 3.3 is the first release that completley implements the c++11 standard.
		# However, most of c++11 was implemented in previous versions.
		MESSAGE("clang ${CMAKE_CXX_COMPILER_VERSION} does not completely support c++11. This may cause some problems in the future. We recommend upgrading to clang-3.3 (Xcode 5.0) or greater.")
	ENDIF (NOT (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER CLANG_MIN_VERSION OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL CLANG_MIN_VERSION))
	# compiler is clang 3.3 or higher. Force c++11 and use libc++.
	IF (XCODE_VERSION)
		SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD c++11)
		SET(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY libc++)
	ELSE (XCODE_VERSION)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
	ENDIF (XCODE_VERSION)
	UNSET(CLANG_MIN_VERSION)
ELSEIF (MSVC AND (MSVC_VERSION GREATER 1699))
		MESSAGE("Supported Visual Studio!")
ELSE ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	MESSAGE(FATAL_ERROR "Your C++ compiler does not support C++11.")
ENDIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
