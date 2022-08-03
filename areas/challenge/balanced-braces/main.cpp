// Programming challenge offered during Playground Games interview: write a 
// function that takes a string as input and determines if it contains only 
// balanced braces. Each opening brace '{', '(', '[' must be matched by a 
// corresponding closing brace  '}', ')', ']'. If there's anything other 
// than braces, it's not balanced.

#include <string>
#include <vector>

bool AreBalancedBraces(const std::string& s)
{
    if (s.empty())
    {
        return false;
    }
    std::vector<char> expectedClosing;
    for (auto c : s)
    {
        if (c == '(')
        {
            expectedClosing.push_back(')');
        }
        else if (c == '[')
        {
            expectedClosing.push_back(']');
        }
        else if (c == '{')
        {
            expectedClosing.push_back('}');
        }
        else if (expectedClosing.empty())
        {
            return false;
        }
        else if (c == expectedClosing.back())
        {
            expectedClosing.pop_back();
        }
        else
        {
            return false;
        }
    }
    return expectedClosing.empty();
}

#include <gtest/gtest.h>

using testing::Test;

struct BalancedBracesFixture : public Test
{
    BalancedBracesFixture()
    {

    }

    ~BalancedBracesFixture()
    {
    }
};

TEST_F(BalancedBracesFixture, T1)
{
    EXPECT_TRUE(AreBalancedBraces("{}"));
    EXPECT_TRUE(AreBalancedBraces("{()[]}(({}()){})"));
    EXPECT_TRUE(AreBalancedBraces("{()[]}{}((({(){[]{}{}}})))(({}()){})"));
    EXPECT_FALSE(AreBalancedBraces("{([)][]}(({}()){})"));
    EXPECT_FALSE(AreBalancedBraces(""));
    EXPECT_FALSE(AreBalancedBraces("{([a])}"));
    EXPECT_FALSE(AreBalancedBraces("{{{()[]}(({}()){})"));
    EXPECT_FALSE(AreBalancedBraces("{()[]}(({}()){})))"));
}
