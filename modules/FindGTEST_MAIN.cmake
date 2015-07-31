# Find gtest_main
# Find the gtest_main library
#
#  GTEST_MAIN_LIBRARIES    - List of libraries when using gtest_main.
#  GTEST_MAIN_FOUND        - True if gtest_main found.
#
# Based on the FindSOIL.cmake module.

IF (GTEST_MAIN_INCLUDE_DIR)
  # Already in cache, be silent
  SET(GTEST_MAIN_FIND_QUIETLY TRUE)
ENDIF (GTEST_MAIN_INCLUDE_DIR)


FIND_LIBRARY(   GTEST_MAIN_LIBRARY
    NAMES gtest_main
    HINTS
        $ENV{GTEST_ROOT}
        ${GTEST_ROOT}
    PATHS /usr/lib /usr/lib64 /usr/lib32 /usr/local/lib
    PATH_SUFFIXES x86 x64 amd64 lib64)

# Per-recommendation
SET(GTEST_MAIN_LIBRARIES    "${GTEST_MAIN_LIBRARY}")

# handle the QUIETLY and REQUIRED arguments and set GTEST_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GTEST_MAIN DEFAULT_MSG GTEST_MAIN_LIBRARIES )

MARK_AS_ADVANCED( GTEST_MAIN_LIBRARY )
