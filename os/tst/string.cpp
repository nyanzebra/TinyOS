#include "gtest/gtest.h"
#include "string.hpp"
#include <string>

TEST(strlen, Nullptr) {
  auto res = tiny_os::strlen(nullptr);
  EXPECT_EQ(res, 0);
}

TEST(strlen, Empty) {
  std::string src = "";
  auto res = tiny_os::strlen(src.c_str());
  EXPECT_EQ(res, 0);
}

TEST(strlen, One) {
  std::string src = "1";
  auto res = tiny_os::strlen(src.c_str());
  EXPECT_EQ(res, 1);
}

TEST(strlen, Many) {
  std::string src = "This is a sentence";
  auto res = tiny_os::strlen(src.c_str());
  EXPECT_EQ(res, 18);
}

TEST(strcpy, NullptrSrc) {
  std::string dst = "1";
  char* dstp = const_cast<char*>(dst.data());
  tiny_os::strcpy(nullptr, dstp);
  EXPECT_STREQ(dst.c_str(), "1");
}
