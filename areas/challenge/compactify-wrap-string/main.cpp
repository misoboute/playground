// Remove all space from the input string and wrap it into lines of max n 
// characters. The last line shouldn't end in a newline char. My solution
// might have extra newline at the end of some lines.

#include <iostream>
#include <sstream>
#include <string>

std::string CompactifyWrapString(int n, const std::string& str)
{
    std::ostringstream out;
        
    int cnt = 0;
    for (auto c : str)
    {
        if (!std::isspace(c))
        {
            if (cnt++ == n)
            {
                out << '\n';
                cnt = 1;
            }
            out << c;
        }
    }
    auto outStr = out.str();
    return outStr;
}

#include <gtest/gtest.h>

using testing::Test;

struct CompactifyWrapStringFixture : public Test
{
    CompactifyWrapStringFixture()
    {

    }

    ~CompactifyWrapStringFixture()
    {
    }
};

TEST_F(CompactifyWrapStringFixture, T1)
{
    EXPECT_EQ(CompactifyWrapString(3, " 1 2 3 4 5 6 7 8 9 0 "), "123\n456\n789\n0");
}

TEST_F(CompactifyWrapStringFixture, T2)
{
    EXPECT_EQ(CompactifyWrapString(2, " 1 2 3 4 5 6 7 8 9 0 "), "12\n34\n56\n78\n90");
}

TEST_F(CompactifyWrapStringFixture, T3)
{
    EXPECT_EQ(CompactifyWrapString(4, " 1 2 3 4 5 6 7 8 9 012 "), "1234\n5678\n9012");
}

TEST_F(CompactifyWrapStringFixture, T4)
{
    EXPECT_EQ(CompactifyWrapString(4, "   "), "");
}

TEST_F(CompactifyWrapStringFixture, T5)
{
    EXPECT_EQ(CompactifyWrapString(4, ""), "");
}
