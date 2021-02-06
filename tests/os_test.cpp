/**
 * Unit tests of VComputer
 */

#include "os.hpp"

#include <gtest/gtest.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>

TEST(OS_class_test, Constructor) {
  std::printf("Hello world\n");
  tec::OS os();

  ASSERT_EQ(0, 0);

}

