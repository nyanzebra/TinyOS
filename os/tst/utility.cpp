#include "gtest/gtest.h"
#include "utility.hpp"

//using ::testing::Return;

TEST(memcpy, SourceToDestination) {
  char* src = "src";
  char* dst = "dst";
  tiny_os::memcpy(src, dst);
  EXPECT_STREQ(src, dst);
}

TEST(memcpy, DifferentTypesSourceToDestination) {
  char* a = "a";
  int b = 'b';
  int* ai = &b;
  tiny_os::memcpy(a, ai);
  EXPECT_EQ(*ai, 'a');
}
