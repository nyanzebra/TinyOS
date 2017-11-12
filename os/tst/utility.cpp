#include "gtest/gtest.h"
#include "utility.hpp"
#include <string>

TEST(memcpy, SourceToDestination) {
  std::string src = "src";
  std::string dst = "dst";
  char* dstp = const_cast<char*>(dst.data());
  tiny_os::memcpy(src.c_str(), dstp);
  EXPECT_STREQ(src.c_str(), dst.c_str());
}

TEST(memcpy, DifferentTypesSourceToDestination) {
  char a = 'a';
  char* az = &a;
  int b = 'b';
  int* ai = &b;
  tiny_os::memcpy(az, ai);
  EXPECT_EQ(*ai, 'a');
}
