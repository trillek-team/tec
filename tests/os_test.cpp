/**
 * Unit tests of VComputer
 */

#include "os.hpp"

#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

TEST(OS_class_test, Constructor) {
	std::printf("Hello world\n");
	tec::OS os();

	ASSERT_EQ(0, 0);
}
