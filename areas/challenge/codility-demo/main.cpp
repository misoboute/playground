#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

using testing::Test;

int solution(std::vector<int> &A)
{
    const std::unordered_set<int> existing(A.cbegin(), A.cend());
    for (auto i = 1; i <= 100001; ++i)
    {
        if (existing.find(i) == existing.cend())
        {
            return i;
        }
    }
}

struct CodilityDemoFixture : public Test
{
    using Vec = std::vector<int>;

    CodilityDemoFixture()
    {
    }

    ~CodilityDemoFixture()
    {
    }
};

TEST_F(CodilityDemoFixture, T1)
{
    EXPECT_EQ(solution(Vec{1, 2, 4, 6}), 3);
}

TEST_F(CodilityDemoFixture, T2)
{
    EXPECT_EQ(solution(Vec{ 1, 2, 3, 4, 5, 7, 4, 6 }), 8);
}

TEST_F(CodilityDemoFixture, T3)
{
    EXPECT_EQ(solution(Vec{ 7, 7, 65, 4, 3, 2, 1, 6}), 5);
}

TEST_F(CodilityDemoFixture, T4)
{
    EXPECT_EQ(solution(Vec{ }), 1);
}
