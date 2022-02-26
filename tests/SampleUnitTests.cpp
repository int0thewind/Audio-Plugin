//
// Created by Hanzhi Yin on 2/26/22.
//

#include <gtest/gtest.h>

TEST(SampleUnitTest, ExpectNumberEqual) { EXPECT_EQ(1, 1); }

TEST(SampleUnitTest, ExpectStringEqual) { EXPECT_STREQ("abc", "abc"); }

TEST(SampleUnitTest, AssertNumberEqual) { ASSERT_EQ(1, 1); }

TEST(SampleUnitTest, AssertStringEqual) { ASSERT_EQ("abc", "abc"); }
