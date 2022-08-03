// Programming challenge offered during Playground Games interview: without 
// using the standard library and using as few comparisons as possible find 
// the sum of the minimun and maximum of a vector of integers.

#include <vector>

int MinMaxSum(const std::vector<int>& v)
{
    if (v.empty())
    {
        return 0;
    }
    auto min = v[0], max = v[0];
    for (std::size_t i = 1; i < v.size(); ++i)
    {
        if (min > v[i])
        {
            min = v[i];
        }
        if (max < v[i])
        {
            max = v[i];
        }
    }
    return min + max;
}

#include <gtest/gtest.h>

using testing::Test;

struct MinMaxSumFixture : public Test
{
    MinMaxSumFixture()
    {

    }

    ~MinMaxSumFixture()
    {
    }
};

TEST_F(MinMaxSumFixture, T1)
{
    EXPECT_EQ(MinMaxSum({}), 0);
}

TEST_F(MinMaxSumFixture, T2)
{
    EXPECT_EQ(MinMaxSum({ 1 }), 2);
}

TEST_F(MinMaxSumFixture, T3)
{
    EXPECT_EQ(MinMaxSum({ 2, 4 }), 6);
}

TEST_F(MinMaxSumFixture, T4)
{
    EXPECT_EQ(MinMaxSum({ 5, 2, -3, 0, 1, -5, 2, 8 }), 3);
}

TEST_F(MinMaxSumFixture, T5)
{
    EXPECT_EQ(MinMaxSum({ 5, 2, 7, 0, 1, 9, 2, 1 }), 9);
}

TEST_F(MinMaxSumFixture, T6)
{
    EXPECT_EQ(MinMaxSum({ -2, -6, -3, -8 }), -10);
}
