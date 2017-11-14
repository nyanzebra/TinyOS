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

TEST(strcpy, NullptrSource) {
	std::string dst = "1";
	char* dstp = const_cast<char*>(dst.data());
	tiny_os::strcpy(nullptr, dstp);
	EXPECT_STREQ(dst.c_str(), "1");
}

TEST(strcpy, EmptySource) {
	std::string dst = "1";
	char* dstp = const_cast<char*>(dst.data());
	tiny_os::strcpy("", dstp);
	EXPECT_STREQ(dst.c_str(), "1");
}

TEST(strcpy, EmptyDestination) {
	std::string src = "1";
	char* srcp = const_cast<char*>(src.data());
	std::string dst = "";
	char* dstp = const_cast<char*>(dst.data());
	tiny_os::strcpy(srcp, dstp);
	EXPECT_STREQ(src.c_str(), "1");
}

TEST(strcpy, SourceToDestination) {
	std::string src = "1";
	char* srcp = const_cast<char*>(src.data());
	std::string dst = "0";
	char* dstp = const_cast<char*>(dst.data());
	tiny_os::strcpy(srcp, dstp);
	EXPECT_STREQ(dst.c_str(), src.c_str());
}

TEST(strrev, Nullptr) {
	tiny_os::strrev(nullptr);
	EXPECT_TRUE(true); // should do nothing
}

TEST(strrev, Empty) {
	std::string str = "";
	char* strp = const_cast<char*>(str.data());
	tiny_os::strrev(strp);
	EXPECT_STREQ(str.c_str(), "");
}

TEST(strrev, String) {
	std::string str = "cat";
	char* strp = const_cast<char*>(str.data());
	tiny_os::strrev(strp);
	EXPECT_STREQ(str.c_str(), "tac");
}

TEST(atoi, Nullptr) {
	auto res = tiny_os::atoi(nullptr);
	EXPECT_EQ(res, 0);
}

TEST(atoi, Empty) {
	std::string str = "";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoi(strp);
	EXPECT_EQ(res, 0);
}

TEST(atoi, PositiveString) {
	std::string str = "123";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoi(strp);
	EXPECT_EQ(res, 123);
}

TEST(atoi, NegativeString) {
	std::string str = "-123";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoi(strp);
	EXPECT_EQ(res, -123);
}

TEST(itoa, Negative) {
	char buffer[20];
	auto res = tiny_os::itoa(-123, buffer);
	EXPECT_STREQ(res, "-123");
}

TEST(itoa, Zero) {
	char buffer[20];
	auto res = tiny_os::itoa(0, buffer);
	EXPECT_STREQ(res, "0");
}

TEST(itoa, Positive) {
	char buffer[20];
	auto res = tiny_os::itoa(123, buffer);
	EXPECT_STREQ(res, "123");
}
