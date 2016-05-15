// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

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

