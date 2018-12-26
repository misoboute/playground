// you can use includes, for example:
#include <limits>
#include <set>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <gtest/gtest.h>

using testing::Test;

int solution(std::vector<int> &A) {
    std::multiset<int> nums(A.cbegin(), A.cend());
    auto prevNum = *nums.cbegin();
    auto currentNumItr = nums.cbegin();
    auto minDist = std::numeric_limits<int>::max();
    for (++currentNumItr; currentNumItr != nums.cend(); ++currentNumItr)
    {
        auto currentDist = *currentNumItr - prevNum;
        if (currentDist < minDist)
        {
            minDist = currentDist;
        }
        prevNum = *currentNumItr;
    }
    return minDist;
}

struct CyberComChal3Fixture : public Test
{
    using Vec = std::vector<int>;

    CyberComChal3Fixture()
    {
    }

    ~CyberComChal3Fixture()
    {
    }
};

TEST_F(CyberComChal3Fixture, T1)
{
    Vec testSet(100000);
    for (auto i = 1; i <= 100000; ++i)
    {
        testSet[i - 1] = i;
    }
    EXPECT_EQ(solution(testSet), 1);
}

TEST_F(CyberComChal3Fixture, T2)
{
    Vec testSet(100000);
    for (auto i = 100000; i >= 1; --i)
    {
        testSet[i - 1] = i;
    }
    EXPECT_EQ(solution(testSet), 1);
}

TEST_F(CyberComChal3Fixture, T3)
{
    Vec testSet(100000);
    for (auto i = 100000; i >= 1; --i)
    {
        testSet[i - 1] = i;
    }
    testSet.push_back(100000);
    EXPECT_EQ(solution(testSet), 0);
}

TEST_F(CyberComChal3Fixture, T4)
{
    Vec testSet{ 1, 1, 1, 1, 1, 1, 1 };
    EXPECT_EQ(solution(testSet), 0);
}

TEST_F(CyberComChal3Fixture, T5)
{
    Vec testSet{ 1, 5, 1, 1, 1, 1, 1, 1 };
    EXPECT_EQ(solution(testSet), 0);
}

TEST_F(CyberComChal3Fixture, T6)
{
    Vec testSet{ 1, 2, 3, 4, 5, 1 };
    EXPECT_EQ(solution(testSet), 0);
}

TEST_F(CyberComChal3Fixture, T7)
{
    Vec testSet(10);
    for (auto i = 10; i >= 1; --i)
    {
        testSet[i - 1] = i;
    }
    testSet.push_back(10);
    EXPECT_EQ(solution(testSet), 0);
}

TEST_F(CyberComChal3Fixture, T8)
{
    Vec testSet{ 4, 3, 2, 1, 4 };
    EXPECT_EQ(solution(testSet), 0);
}

TEST_F(CyberComChal3Fixture, T9)
{
    Vec testSet(21);
    for (auto i = -10; i <= 10; i++)
    {
        testSet[i + 10] = i * i * i;
    }
    EXPECT_EQ(solution(testSet), 1);
}

TEST_F(CyberComChal3Fixture, T10)
{
    Vec testSet(21);
    for (auto i = -10; i <= 10; i++)
    {
        testSet[i + 10] = 3 * i * i * i;
    }
    EXPECT_EQ(solution(testSet), 3);
}
