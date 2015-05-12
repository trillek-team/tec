# - Find the OpenGL Extension Wrangler Library (GLEW)
# This module defines the following variables:
#  GLEW_INCLUDE_DIRS - include directories for GLEW
#  GLEW_LIBRARIES - libraries to link against GLEW
#  GLEW_FOUND - true IF GLEW has been found and can be used

#=============================================================================
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_path(GLEW_INCLUDE_DIR GL/glew.h)
SET(GLEW_DEBUG_NAMES glew32_debug)
SET(GLEW_NAMES glew32 GLEW)
add_definitions(-DGLEW_STATIC)

# Reset the variables so that find_library will rescan in case we changed from static to none (or vice versa).
UNSET(GLEW_LIBRARY)
UNSET(GLEW_DEBUG_LIBRARY)

FIND_LIBRARY(GLEW_LIBRARY NAMES ${GLEW_NAMES} PATH_SUFFIXES x64 amd64 lib64)
FIND_LIBRARY(GLEW_DEBUG_LIBRARY NAMES ${GLEW_DEBUG_NAMES} PATH_SUFFIXES x64 amd64 lib64)

IF(NOT GLEW_DEBUG_LIBRARY AND GLEW_LIBRARY)
	SET(GLEW_DEBUG_LIBRARY ${GLEW_LIBRARY})
ENDIF(NOT GLEW_DEBUG_LIBRARY AND GLEW_LIBRARY)

SET(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
SET(GLEW_LIBRARIES debug ${GLEW_DEBUG_LIBRARY} optimized ${GLEW_LIBRARY})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(  GLEW
									DEFAULT_MSG
									GLEW_LIBRARIES
									GLEW_INCLUDE_DIR
									)

MARK_AS_ADVANCED(GLEW_INCLUDE_DIR GLEW_LIBRARY GLEW_DEBUG_LIBRARY)
