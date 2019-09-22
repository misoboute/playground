
#include <gtest/gtest.h>

using testing::Test;

struct CafeBazaarFixture : public Test
{
    using Vec = std::vector<int>;

    CafeBazaarFixture()
    {
    }

    ~CafeBazaarFixture()
    {
    }
};

TEST_F(CafeBazaarFixture, T1)
{
}
