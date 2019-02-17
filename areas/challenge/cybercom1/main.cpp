// you can use includes, for example:

#include <sstream>
#include <string>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;

#include <gtest/gtest.h>

using testing::Test;

int solution(int A, int B) {
    std::ostringstream decim1Stream, decim2Stream;
    decim1Stream << A;
    decim2Stream << B;
    auto pos = decim2Stream.str().find(decim1Stream.str());
    return pos == std::string::npos ? -1 : static_cast<int>(pos);
}

struct CyberComChal1Fixture : public Test
{
    using Vec = std::vector<int>;

    CyberComChal1Fixture()
    {
    }

    ~CyberComChal1Fixture()
    {
    }
};

TEST_F(CyberComChal1Fixture, T1)
{
    EXPECT_EQ(solution(0, 12355), -1);
}

TEST_F(CyberComChal1Fixture, T2)
{
    EXPECT_EQ(solution(0, 1231), -1);
}

TEST_F(CyberComChal1Fixture, T3)
{
    EXPECT_EQ(solution(1, 1231), 0);
}

TEST_F(CyberComChal1Fixture, T4)
{
    EXPECT_EQ(solution(2, 546211), 3);
}

TEST_F(CyberComChal1Fixture, T5)
{
    EXPECT_EQ(solution(1, 546211), 4);
}

TEST_F(CyberComChal1Fixture, T6)
{
    EXPECT_EQ(solution(899999999, 999999999), -1);
}

TEST_F(CyberComChal1Fixture, T7)
{
    EXPECT_EQ(solution(99999999, 999999999), 0);
}

TEST_F(CyberComChal1Fixture, T8)
{
    EXPECT_EQ(solution(99999999, 899999999), 1);
}

TEST_F(CyberComChal1Fixture, T9)
{
    EXPECT_EQ(solution(9999999, 799999998), 1);
}

TEST_F(CyberComChal1Fixture, T10)
{
    EXPECT_EQ(solution(9999999, 879999999), 2);
}

TEST_F(CyberComChal1Fixture, T11)
{
    EXPECT_EQ(solution(1, 0), -1);
}

TEST_F(CyberComChal1Fixture, T12)
{
    EXPECT_EQ(solution(1, 10), 0);
}

TEST_F(CyberComChal1Fixture, T13)
{
    EXPECT_EQ(solution(10, 100), 0);
}

TEST_F(CyberComChal1Fixture, T14)
{
    EXPECT_EQ(solution(10, 9710010), 2);
}
