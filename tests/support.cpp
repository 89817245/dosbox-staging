/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *
 *  Copyright (C) 2020-2021  The DOSBox Staging Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "support.h"

#include <cstdint>
#include <gtest/gtest.h>

#include <string>
#include <sys/types.h>

namespace {

TEST(DriveIndex, DriveA)
{
	EXPECT_EQ(0, drive_index('a'));
	EXPECT_EQ(0, drive_index('A'));
}

TEST(DriveIndex, DriveZ)
{
	EXPECT_EQ(25, drive_index('z'));
	EXPECT_EQ(25, drive_index('Z'));
}

TEST(Support_split_delim, NoBoundingDelims)
{
	const std::vector<std::string> expected({"a", "/b", "/c/d", "/e/f/"});
	EXPECT_EQ(split("a:/b:/c/d:/e/f/", ':'), expected);
	EXPECT_EQ(split("a /b /c/d /e/f/", ' '), expected);
	EXPECT_EQ(split("abc", 'x'), std::vector<std::string>{"abc"});
}

TEST(Support_split_delim, DelimAtStartNotEnd)
{
	const std::vector<std::string> expected({"", "a", "/b", "/c/d", "/e/f/"});
	EXPECT_EQ(split(":a:/b:/c/d:/e/f/", ':'), expected);
	EXPECT_EQ(split(" a /b /c/d /e/f/", ' '), expected);
}

TEST(Support_split_delim, DelimAtEndNotStart)
{
	const std::vector<std::string> expected({"a", "/b", "/c/d", "/e/f/", ""});
	EXPECT_EQ(split("a:/b:/c/d:/e/f/:", ':'), expected);
	EXPECT_EQ(split("a /b /c/d /e/f/ ", ' '), expected);
}

TEST(Support_split_delim, DelimsAtBoth)
{
	const std::vector<std::string> expected({"", "a", "/b", "/c/d", "/e/f/", ""});
	EXPECT_EQ(split(":a:/b:/c/d:/e/f/:", ':'), expected);
	EXPECT_EQ(split(" a /b /c/d /e/f/ ", ' '), expected);
}

TEST(Support_split_delim, MultiInternalDelims)
{
	const std::vector<std::string> expected(
	        {"a", "/b", "", "/c/d", "", "", "/e/f/"});
	EXPECT_EQ(split("a:/b::/c/d:::/e/f/", ':'), expected);
	EXPECT_EQ(split("a /b  /c/d   /e/f/", ' '), expected);
}

TEST(Support_split_delim, MultiBoundingDelims)
{
	const std::vector<std::string> expected(
	        {"", "", "a", "/b", "/c/d", "/e/f/", "", "", ""});
	EXPECT_EQ(split("::a:/b:/c/d:/e/f/:::", ':'), expected);
	EXPECT_EQ(split("  a /b /c/d /e/f/   ", ' '), expected);
}

TEST(Support_split_delim, MixedDelims)
{
	const std::vector<std::string> expected(
	        {"", "", "a", "/b", "", "/c/d", "/e/f/"});
	EXPECT_EQ(split("::a:/b::/c/d:/e/f/", ':'), expected);
	EXPECT_EQ(split("  a /b  /c/d /e/f/", ' '), expected);
}

TEST(Support_split_delim, Empty)
{
	const std::vector<std::string> empty;
	const std::vector<std::string> two({"", ""});
	const std::vector<std::string> three({"", "", ""});

	EXPECT_EQ(split("", ':'), empty);
	EXPECT_EQ(split(":", ':'), two);
	EXPECT_EQ(split("::", ':'), three);
	EXPECT_EQ(split("", ' '), empty);
	EXPECT_EQ(split(" ", ' '), two);
	EXPECT_EQ(split("  ", ' '), three);
}
 
TEST(Support_split, NoBoundingWhitespace)
{
	const std::vector<std::string> expected({"a", "/b", "/c/d", "/e/f/"});
	EXPECT_EQ(split("a /b /c/d /e/f/"), expected);
	EXPECT_EQ(split("abc"), std::vector<std::string>{"abc"});
}
TEST(Support_split, WhitespaceAtStartNotEnd)
{
	const std::vector<std::string> expected({"a", "/b", "/c/d", "/e/f/"});
	EXPECT_EQ(split(" a /b /c/d /e/f/"), expected);
}

TEST(Support_split, WhitespaceAtEndNotStart)
{
	const std::vector<std::string> expected({"a", "/b", "/c/d", "/e/f/"});
	EXPECT_EQ(split("a /b /c/d /e/f/ "), expected);
}

TEST(Support_split, WhitespaceAtBoth)
{
	const std::vector<std::string> expected({"a", "/b", "/c/d", "/e/f/"});
	EXPECT_EQ(split(" a /b /c/d /e/f/ "), expected);
}

TEST(Support_split, MultiInternalWhitespace)
{
	const std::vector<std::string> expected({"a", "/b", "/c/d", "/e/f/"});
	EXPECT_EQ(split("a /b  /c/d   /e/f/"), expected);
}

TEST(Support_split, MultiBoundingWhitespace)
{
	const std::vector<std::string> expected({"a", "/b", "/c/d", "/e/f/"});
	EXPECT_EQ(split("  a /b /c/d /e/f/   "), expected);
}

TEST(Support_split, MixedWhitespace)
{
	const std::vector<std::string> expected({"a", "b", "c"});
	EXPECT_EQ(split("\t\na\f\vb\rc"), expected);
	EXPECT_EQ(split("a\tb\f\vc"), expected);
	EXPECT_EQ(split(" a \n \v \r b \f \r c "), expected);
}

TEST(Support_split, Empty)
{
	const std::vector<std::string> empty;
	EXPECT_EQ(split(""), empty);
	EXPECT_EQ(split(" "), empty);
	EXPECT_EQ(split("   "), empty);
}

TEST(Support_left_shift_signed, PositiveValid)
{
	// shifting zero ...
	int8_t val_8bit = 0;
	int16_t val_16bit = 0;
	int32_t val_32bit = 0;
	// by zero
	EXPECT_EQ(left_shift_signed(val_8bit, 0), 0);
	EXPECT_EQ(left_shift_signed(val_16bit, 0), 0);
	EXPECT_EQ(left_shift_signed(val_32bit, 0), 0);

	// shifting one ...
	val_8bit = 1;
	val_16bit = 1;
	val_32bit = 1;

	// by four
	EXPECT_EQ(left_shift_signed(val_8bit, 4), 16);
	EXPECT_EQ(left_shift_signed(val_16bit, 4), 16);
	EXPECT_EQ(left_shift_signed(val_32bit, 4), 16);

	// by max signed bits
	EXPECT_EQ(left_shift_signed(val_8bit, 6), 64);
	EXPECT_EQ(left_shift_signed(val_16bit, 14), 16384);
	EXPECT_EQ(left_shift_signed(val_32bit, 30), 1073741824);

	// max shiftable value before overflow
	val_8bit = INT8_MAX / 2;
	val_16bit = INT16_MAX / 2;
	val_32bit = INT32_MAX / 2;

	EXPECT_EQ(left_shift_signed(val_8bit, 1), INT8_MAX - 1);
	EXPECT_EQ(left_shift_signed(val_16bit, 1), INT16_MAX - 1);
	EXPECT_EQ(left_shift_signed(val_32bit, 1), INT32_MAX - 1);
}

TEST(Support_left_shift_signed, NegativeValid)
{
	// shifting negative one ...
	int8_t val_8bit = -1;
	int16_t val_16bit = -1;
	int32_t val_32bit = -1;

	// by four
	EXPECT_EQ(left_shift_signed(val_8bit, 4), -16);
	EXPECT_EQ(left_shift_signed(val_16bit, 4), -16);
	EXPECT_EQ(left_shift_signed(val_32bit, 4), -16);

	// by max signed bits
	EXPECT_EQ(left_shift_signed(val_8bit, 7), INT8_MIN);
	EXPECT_EQ(left_shift_signed(val_16bit, 15), INT16_MIN);
	EXPECT_EQ(left_shift_signed(val_32bit, 31), INT32_MIN);

	// max shiftable value before overflow
	val_8bit = INT8_MIN / 2;
	val_16bit = INT16_MIN / 2;
	val_32bit = INT32_MIN / 2;

	EXPECT_EQ(left_shift_signed(val_8bit, 1), INT8_MIN);
	EXPECT_EQ(left_shift_signed(val_16bit, 1), INT16_MIN);
	EXPECT_EQ(left_shift_signed(val_32bit, 1), INT32_MIN);
}

TEST(Support_left_shift_signed, Invalid)
{
	// Overflow (positive)
	int8_t val_8bit = INT8_MAX;
	int16_t val_16bit = INT16_MAX;
	int32_t val_32bit = INT32_MAX;

	EXPECT_DEBUG_DEATH({ left_shift_signed(val_8bit, 1); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_16bit, 1); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_32bit, 1); }, "");

	val_8bit = 1;
	val_16bit = 1;
	val_32bit = 1;

	EXPECT_DEBUG_DEATH({ left_shift_signed(val_8bit, 7); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_16bit, 15); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_32bit, 31); }, "");

	// Overflow (negative)
	val_8bit = INT8_MIN;
	val_16bit = INT16_MIN;
	val_32bit = INT32_MIN;

	EXPECT_DEBUG_DEATH({ left_shift_signed(val_8bit, 1); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_16bit, 1); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_32bit, 1); }, "");

	val_8bit = -1;
	val_16bit = -1;
	val_32bit = -1;

	EXPECT_DEBUG_DEATH({ left_shift_signed(val_8bit, 8); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_16bit, 16); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_32bit, 32); }, "");

	// Shift a negative number of bits
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_8bit, -1); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_16bit, -100); }, "");
	EXPECT_DEBUG_DEATH({ left_shift_signed(val_32bit, -10000); }, "");
}

} // namespace
