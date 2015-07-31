# Find gtest
# Find the gtest includes and library
#
#  GTEST_INCLUDE_DIRS - where to find gtest/gtest.h, etc.
#  GTEST_LIBRARIES    - List of libraries when using gtest.
#  GTEST_FOUND        - True if gtest found.
#
# Based on the FindSOIL.cmake module.

IF (GTEST_INCLUDE_DIR)
  # Already in cache, be silent
  SET(GTEST_FIND_QUIETLY TRUE)
ENDIF (GTEST_INCLUDE_DIR)

FIND_PATH(  GTEST_INCLUDE_DIR
    NAMES gtest/gtest.h
    HINTS
        $ENV{GTEST_ROOT}/include
        ${GTEST_ROOT}/include
    PATHS /usr/install
    PATH_SUFFIXES gtest-1.7.0/include gtest-1.7.0)

FIND_LIBRARY(   GTEST_LIBRARY
    NAMES gtest
    HINTS
        $ENV{GTEST_ROOT}
        ${GTEST_ROOT}
    PATHS /usr/lib /usr/lib64 /usr/lib32 /usr/local/lib
    PATH_SUFFIXES x86 x64 amd64 lib64)

# Per-recommendation
SET(GTEST_INCLUDE_DIRS "${GTEST_INCLUDE_DIR}")
SET(GTEST_LIBRARIES    "${GTEST_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set GTEST_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GTEST DEFAULT_MSG GTEST_LIBRARIES GTEST_INCLUDE_DIRS)

include_directories("${GTEST_INCLUDE_DIRS}")
MARK_AS_ADVANCED( GTEST_LIBRARY GTEST_INCLUDE_DIR )
