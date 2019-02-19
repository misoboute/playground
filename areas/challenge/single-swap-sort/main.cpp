#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

using namespace std;
using testing::Test;

bool isSorted(vector<int> &A)
{
    const auto n = A.size();
    for (auto i = 1; i < n; ++i)
    {
        if (A[i] < A[i - 1])
        {
            return false;
        }
    }
    return true;
}

bool solution(vector<int> &A) {
    const auto n = A.size();
    vector<size_t> dropI;
    for (auto i = 1; i < n; ++i)
    {
        if (A[i] < A[i - 1])
        {
            dropI.push_back(i);
            if (dropI.size() > 2)
            {
                return false;
            }
        }
    }
    if (dropI.size() == 2)
    {
        auto d1 = dropI[0], d2 = dropI[1];
        std::swap(A[d1 - 1], A[d2]);
        return isSorted(A);
    }
    else if (dropI.size() == 1)
    {
        auto d1 = dropI[0];
        for (auto i = n; i > d1  - 1; i < n; i++)
        {
            if (A[i] <= )
        }
        std::swap(A[d1 - 1], A[d2]);
        return isSorted(A);
        return false;
    }
    return true;
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
    auto input = vector<int>{ 1, 5, 3, 3, 7 };
    EXPECT_EQ(solution(input), true);
}

TEST_F(TomTomFixture, T2)
{
    auto input = vector<int>{ 1, 3, 5, 3, 4 };
    EXPECT_EQ(solution(input), false);
}

TEST_F(TomTomFixture, T3)
{
    auto input = vector<int>{ 1, 3, 5 };
    EXPECT_EQ(solution(input), true);
}

TEST_F(TomTomFixture, T4)
{
    auto input = vector<int>{ 4, 5, 3, 3, 7 };
    EXPECT_EQ(solution(input), false);
}

TEST_F(TomTomFixture, T5)
{
    auto input = vector<int>{ 1, 5, 3, 1, 7 };
    EXPECT_EQ(solution(input), true);
}

TEST_F(TomTomFixture, T6)
{
    auto input = vector<int>{ 8, 2, 3, 4, 5, 6, 7, 1 };
    EXPECT_EQ(solution(input), true);
}

TEST_F(TomTomFixture, T7)
{
    auto input = vector<int>{ 8, 2, 3, 4, 5, 6, 7, 1, 9 };
    EXPECT_EQ(solution(input), true);
}

TEST_F(TomTomFixture, T8)
{
    auto input = vector<int>{ 0, 8, 2, 3, 4, 5, 6, 7, 1, 9 };
    EXPECT_EQ(solution(input), true);
}

TEST_F(TomTomFixture, T9)
{
    auto input = vector<int>{ 8, 2, 3, 4, 5, 6, 7, 8 };
    EXPECT_EQ(solution(input), false);
}

TEST_F(TomTomFixture, T10)
{
    auto input = vector<int>{ 1, 2, 3, 4, 5, 6, 1, 8 };
    EXPECT_EQ(solution(input), false);
}

// TEST_F(TomTomFixture, T10)
// {
//     auto input = vector<int>{ 1, 2, 3, 4, 5, 6, 1, 8 };
//     EXPECT_EQ(solution(input), false);
// }
