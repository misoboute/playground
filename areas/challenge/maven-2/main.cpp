#include <algorithm>
#include <numeric>

#include <gtest/gtest.h>

using testing::Test;

int solution(std::vector<int> &A)
{
    // write your code in C++14 (g++ 6.2.0)
    const auto n = A.size();
    if (n < 2)
    {
        return 0;
    }
    std::sort(A.begin(), A.end());
    unsigned long long int totalTime = A[0] + A[1];
    for (size_t i = 2; i < n; ++i)
    {
        totalTime = 2 * totalTime + A[i];
    }
    return totalTime;
}

struct TomTomFixture : public Test
{
    TomTomFixture()
    {
    }

    ~TomTomFixture()
    {
    }
};

TEST_F(TomTomFixture, T2)
{
    std::vector<int> a { 6, 3, 1 };
    EXPECT_EQ(solution(a), 14);
}

TEST_F(TomTomFixture, T3)
{
    std::vector<int> a { 16, 6, 3, 1 };
    EXPECT_EQ(solution(a), 44);
}

TEST_F(TomTomFixture, T4)
{
    std::vector<int> a { 16, 6, 23, 3, 1 };
    EXPECT_EQ(solution(a), 111);
}

TEST_F(TomTomFixture, ZeroLength)
{
    std::vector<int> a { };
    EXPECT_EQ(solution(a), 0);
}

TEST_F(TomTomFixture, OneList)
{
    std::vector<int> a { 987 };
    EXPECT_EQ(solution(a), 0);
}
