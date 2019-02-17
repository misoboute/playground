#include <vector>

#include <gtest/gtest.h>

using testing::Test;

int solution(std::vector<int> &A) {
    // write your code in C++14 (g++ 6.2.0)
    const auto n = A.size();
    auto count = 0;
    for (size_t low = 0; low < n; ++low)
    {
        long long int sum = 0;
        for (size_t high = low; high < n; ++high)
        {
            sum += A[high];
            if (sum == 0)
            {
                if (++count > 1000000000)
                {
                    return -1;
                }
            }
        }
    }
    return count;
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
    std::vector<int> a(100000, 0);
    auto num = 5000;
    for (int i = 0; i < 100000; ++i)
    {
        a[i] = (num *= -1);
    }
    EXPECT_EQ(solution(a), -1);
}
