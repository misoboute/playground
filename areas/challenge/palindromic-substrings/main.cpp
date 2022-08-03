// 647. Palindromic Substrings (Medium)
// Given a string s, return the number of palindromic substrings in it.
// A string is a palindrome when it reads the same backward as forward.
// A substring is a contiguous sequence of characters within the string.

// Example 1:
// Input: s = "abc"
// Output: 3
// Explanation: Three palindromic strings: "a", "b", "c".

// Example 2:
// Input: s = "aaa"
// Output: 6
// Explanation: Six palindromic strings: "a", "a", "a", "aa", "aa", "aaa".
 
// Constraints:
// 1 <= s.length <= 1000
// s consists of lowercase English letters.

#include <string>

class Solution
{
public:
    int countSubstrings(const std::string& s)
    {
        auto count = s.size();
        const auto lastButOneIndex = s.size() - 1;
        for (std::size_t i = 1; i < lastButOneIndex; ++i)
        {
            for (std::size_t j = 1;
                j < s.size() - i && j <= i && s[i + j] == s[i - j];
                ++j, ++count);
        }
        for (std::size_t i = 0; i < s.size(); ++i)
        {
            for (std::size_t j = 0; 
                j < s.size() - i - 1 && j <= i && s[i + j + 1] == s[i - j]; 
                ++j, ++count);
        }
        return count;
    }
};

#include <gtest/gtest.h>

using testing::Test;

struct PalindromicSubstringsFixture : public Test
{
    PalindromicSubstringsFixture()
    {
    }

    ~PalindromicSubstringsFixture()
    {
    }

    Solution m_Sol;
};

TEST_F(PalindromicSubstringsFixture, T1)
{
    EXPECT_EQ(m_Sol.countSubstrings("b"), 1);
}

TEST_F(PalindromicSubstringsFixture, T2)
{
    EXPECT_EQ(m_Sol.countSubstrings("a"), 1);
}

TEST_F(PalindromicSubstringsFixture, T3)
{
    EXPECT_EQ(m_Sol.countSubstrings("ab"), 2);
}

TEST_F(PalindromicSubstringsFixture, T4)
{
    EXPECT_EQ(m_Sol.countSubstrings("abc"), 3);
}

TEST_F(PalindromicSubstringsFixture, T5)
{
    EXPECT_EQ(m_Sol.countSubstrings("aaa"), 6);
}

TEST_F(PalindromicSubstringsFixture, T6)
{
    EXPECT_EQ(m_Sol.countSubstrings("aba"), 4);
}

TEST_F(PalindromicSubstringsFixture, T7)
{
    EXPECT_EQ(m_Sol.countSubstrings("abba"), 6);
}

TEST_F(PalindromicSubstringsFixture, T8)
{
    EXPECT_EQ(m_Sol.countSubstrings("abcba"), 7);
}

TEST_F(PalindromicSubstringsFixture, T9)
{
    EXPECT_EQ(m_Sol.countSubstrings("abcbab"), 9);
}

TEST_F(PalindromicSubstringsFixture, T10)
{
    EXPECT_EQ(m_Sol.countSubstrings("abcbabc"), 11);
}

TEST_F(PalindromicSubstringsFixture, T11)
{
    EXPECT_EQ(m_Sol.countSubstrings("abcbabcdc"), 14);
}

TEST_F(PalindromicSubstringsFixture, T12)
{
    EXPECT_EQ(m_Sol.countSubstrings("abcbabcdcaa"), 17);
}

TEST_F(PalindromicSubstringsFixture, T13)
{
    EXPECT_EQ(m_Sol.countSubstrings("ddabcbabcdcaa"), 20);
}
