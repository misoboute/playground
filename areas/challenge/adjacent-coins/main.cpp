
#include <algorithm>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>

using namespace std;
using testing::Test;

int solution(vector<int> & A) {
    int n = A.size();
    int result = 0;
    for (int i = 0; i < n - 1; i++) {
        if (A[i] == A[i + 1])
            result = result + 1;
    }
    int r = 0;
    for (int i = 0; i < n; i++) {
        int count = 0;
        if (i > 0) {
            if (A[i - 1] != A[i])
                count = count + 1;
            else
                count = count - 1;
        }
        if (i < n - 1) {
            if (A[i + 1] != A[i])
                count = count + 1;
            else
                count = count - 1;
        }
        r = max(r, count);
    }
    return result + r;
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

TEST_F(TomTomFixture, T1)
{
    auto input = vector<int>{ 1, 1, 0, 1, 0, 0 };
    EXPECT_EQ(solution(input), 4);
}

TEST_F(TomTomFixture, T2)
{
    auto input = vector<int>{ 1, 1, 1, 1, 0, 1 };
    EXPECT_EQ(solution(input), 5);
}

TEST_F(TomTomFixture, T3)
{
    auto input = vector<int>{ 1 };
    EXPECT_EQ(solution(input), 0);
}

TEST_F(TomTomFixture, T4)
{
    auto input = vector<int>{ 1, 1 };
    EXPECT_EQ(solution(input), 1);
}

TEST_F(TomTomFixture, T5)
{
    auto input = vector<int>{ 1, 0 };
    EXPECT_EQ(solution(input), 1);
}

TEST_F(TomTomFixture, T6)
{
    auto input = vector<int>{ 1, 0, 1, 0, 1, 0 };
    EXPECT_EQ(solution(input), 2);
}

TEST_F(TomTomFixture, T7)
{
    auto input = vector<int>{ 0, 0, 0, 0, 0, 0 };
    EXPECT_EQ(solution(input), 5);
}

TEST_F(TomTomFixture, T8)
{
    auto input = vector<int>(1000, 0);
    EXPECT_EQ(solution(input), 999);
}

TEST_F(TomTomFixture, T9)
{
    auto input = vector<int>{ 0, 0, 1, 0, 0, 1 };
    EXPECT_EQ(solution(input), 4);
}

TEST_F(TomTomFixture, T10)
{
    auto input = vector<int>{ 0, 0, 1, 0, 1, 0 };
    EXPECT_EQ(solution(input), 3);
}

TEST_F(TomTomFixture, T11)
{
    auto input = vector<int>{ 0, 1, 0 };
    EXPECT_EQ(solution(input), 2);
}

TEST_F(TomTomFixture, T12)
{
    auto input = vector<int>{ 1, 1, 0 };
    EXPECT_EQ(solution(input), 2);
}

TEST_F(TomTomFixture, T13)
{
    auto input = vector<int>{ 0, 1, 1 };
    EXPECT_EQ(solution(input), 2);
}

TEST_F(TomTomFixture, T14)
{
    auto input = vector<int>{ 0, 1, 1, 1, 1, 1 };
    EXPECT_EQ(solution(input), 5);
}

TEST_F(TomTomFixture, T15)
{
    auto input = vector<int>{ 0, 1, 1, 1, 1, 0 };
    EXPECT_EQ(solution(input), 4);
}

TEST_F(TomTomFixture, T16)
{
    auto input = vector<int>{ 1, 1, 1, 1, 1, 0 };
    EXPECT_EQ(solution(input), 5);
}
