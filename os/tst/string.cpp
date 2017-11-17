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

TEST(atoi, HexString) {
	std::string str = "0xFF83";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoi(strp);
	EXPECT_EQ(res, 0xFF83);
}

TEST(atoi, FloatString) {
	std::string str = "3.14";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoi(strp);
	EXPECT_EQ(res, 3);
}

TEST(atol, Nullptr) {
	auto res = tiny_os::atol(nullptr);
	EXPECT_EQ(res, 0);
}

TEST(atol, Empty) {
	std::string str = "";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atol(strp);
	EXPECT_EQ(res, 0);
}

TEST(atol, PositiveString) {
	std::string str = "123";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atol(strp);
	EXPECT_EQ(res, 123);
}

TEST(atol, NegativeString) {
	std::string str = "-123";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atol(strp);
	EXPECT_EQ(res, -123);
}

TEST(atol, HexString) {
	std::string str = "0xFF83";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atol(strp);
	EXPECT_EQ(res, 0xFF83);
}

TEST(atol, FloatString) {
	std::string str = "3.14";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atol(strp);
	EXPECT_EQ(res, 3);
}

TEST(atoll, Nullptr) {
	auto res = tiny_os::atoll(nullptr);
	EXPECT_EQ(res, 0);
}

TEST(atoll, Empty) {
	std::string str = "";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoll(strp);
	EXPECT_EQ(res, 0);
}

TEST(atoll, PositiveString) {
	std::string str = "123456789";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoll(strp);
	EXPECT_EQ(res, 123456789);
}

TEST(atoll, NegativeString) {
	std::string str = "-123456789";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoll(strp);
	EXPECT_EQ(res, -123456789);
}

TEST(atoll, HexString) {
	std::string str = "0xFFFFFFFF";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoll(strp);
	EXPECT_EQ(res, 0xFFFFFFFF);
}

TEST(atoll, FloatString) {
	std::string str = "3.14";
	char* strp = const_cast<char*>(str.data());
	auto res = tiny_os::atoll(strp);
	EXPECT_EQ(res, 3);
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

TEST(itoa, Decimal) {
	char buffer[20];
	auto res = tiny_os::itoa(123, buffer, 'd');
	EXPECT_STREQ(res, "123");
}

TEST(itoa, Hex) {
	char buffer[20];
	auto res = tiny_os::itoa(123, buffer, 'x');
	EXPECT_STREQ(res, "7b");
}

TEST(itoa, Float) {
	char buffer[20];
	auto res = tiny_os::itoa(3.14, buffer);
	EXPECT_STREQ(res, "3");
}

TEST(ltoa, Negative) {
	char buffer[20];
	auto res = tiny_os::ltoa(-123, buffer);
	EXPECT_STREQ(res, "-123");
}

TEST(ltoa, Zero) {
	char buffer[20];
	auto res = tiny_os::ltoa(0, buffer);
	EXPECT_STREQ(res, "0");
}

TEST(ltoa, Positive) {
	char buffer[20];
	auto res = tiny_os::ltoa(123, buffer);
	EXPECT_STREQ(res, "123");
}

TEST(ltoa, Decimal) {
	char buffer[20];
	auto res = tiny_os::ltoa(123, buffer, 'd');
	EXPECT_STREQ(res, "123");
}

TEST(ltoa, Hex) {
	char buffer[20];
	auto res = tiny_os::ltoa(123, buffer, 'x');
	EXPECT_STREQ(res, "7b");
}

TEST(ltoa, Float) {
	char buffer[20];
	auto res = tiny_os::ltoa(3.14, buffer);
	EXPECT_STREQ(res, "3");
}

TEST(lltoa, Negative) {
	char buffer[20];
	auto res = tiny_os::lltoa(-123456789, buffer);
	EXPECT_STREQ(res, "-123456789");
}

TEST(lltoa, Zero) {
	char buffer[20];
	auto res = tiny_os::lltoa(0, buffer);
	EXPECT_STREQ(res, "0");
}

TEST(lltoa, Positive) {
	char buffer[20];
	auto res = tiny_os::lltoa(123456789, buffer);
	EXPECT_STREQ(res, "123456789");
}

TEST(lltoa, Decimal) {
	char buffer[20];
	auto res = tiny_os::lltoa(123456789, buffer, 'd');
	EXPECT_STREQ(res, "123456789");
}

TEST(lltoa, Hex) {
	char buffer[20];
	auto res = tiny_os::lltoa(0xFFFFFFFF, buffer, 'x');
	EXPECT_STREQ(res, "ffffffff");
}

TEST(lltoa, Float) {
	char buffer[20];
	auto res = tiny_os::lltoa(3.14, buffer);
	EXPECT_STREQ(res, "3");
}
