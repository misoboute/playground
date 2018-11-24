#include <iostream>

#include <algorithm>
#include <limits>

#include <gtest/gtest.h>

using testing::Test;

const auto maxInt = std::numeric_limits<int>::max();
size_t called = 0;

int minChanges(const std::vector<int>& map, int curSeg, int curLane)
{
    ++called;
    if (called % 10000000 == 0)
        std::cout << "called " << called << "times" << std::endl;
    if (curSeg >= map.size())
        return 0;
    if (map[curSeg] & (1 << curLane))
        return minChanges(map, curSeg + 1, curLane);
    auto minChangesLeft = maxInt, minChangesRight = maxInt;
    if (curLane > 0 && map[curSeg] & (1 << (curLane - 1)))
        minChangesLeft = minChanges(map, curSeg + 1, curLane - 1);
    if (curLane < 7 && map[curSeg] & (1 << (curLane + 1)))
        minChangesRight = minChanges(map, curSeg + 1, curLane + 1);
    if (minChangesLeft == minChangesRight && minChangesLeft == maxInt)
        return maxInt;
    return std::min(minChangesLeft, minChangesRight) + 1;
}

int solution(const std::vector<int>& map)
{
    auto changes = std::vector<int>(8, maxInt);
    for (int i = 0; i < 8; i++)
    {
        if (map[0] & (1 << i))

            changes[i] = minChanges(map, 0, i);

    }
    auto minChanges = *std::min_element(changes.cbegin(), changes.cend());
    return minChanges == maxInt ? -1 : minChanges;
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
    EXPECT_EQ(solution({ 7, 11, 10, 4 }), 1);
}

TEST_F(TomTomFixture, T2)
{
    EXPECT_EQ(solution({ 9, 9, 13, 4, 14, 10, 14, 7, 5, 3, 2, 5, 9, 6, 6 }), 6);
}

TEST_F(TomTomFixture, T3)
{
    EXPECT_EQ(solution(std::vector<int>(size_t(127), 64)), 0);
}

TEST_F(TomTomFixture, T4)
{
    EXPECT_EQ(solution(std::vector<int>(size_t(127), 127)), 0);
}

TEST_F(TomTomFixture, T5)
{
    EXPECT_EQ(solution(std::vector<int>(size_t(127), 128)), 0);
}

TEST_F(TomTomFixture, T6)
{
    EXPECT_EQ(solution(std::vector<int>(size_t(127), 3)), 0);
}

TEST_F(TomTomFixture, T7)
{
    EXPECT_EQ(solution(std::vector<int>(size_t(127), 129)), 0);
}

TEST_F(TomTomFixture, T8)
{
    // 00000001 0
    // 00000010 1
    // 00000100 2 
    // 00001000 3
    // 00010000 4
    // 00100000 5 
    // 01000000 6 
    // 10000000 7
    // 01000000 8
    // 00100000 9
    // 00010000 10
    // 00001000 11
    // 00000100 12
    // 00000010 13
    // 00000001 14
    // 00000010
    // 00000100
    // 00001000
    // ...
    std::vector<int> map(200);
    for (auto i = 0; i < map.size(); ++i)
    {
        auto lane = (i % 14) <= 7 ? ((i % 14) % 8) : 7 - (i % 7);
        map[i] = 1 << lane;
    }
    EXPECT_EQ(solution(map), 199);

}

TEST_F(TomTomFixture, T9)
{
    // 01010101
    // 10101010
    // 01010101
    // 10101010
    // 01010101
    // 10101010
    // ...
    // Fails performance test! Takes forever!
    std::vector<int> map(200);
    for (auto i = 0; i < map.size(); ++i)
        map[i] = 0x55 << (i % 2);
    EXPECT_EQ(solution(map), 199);
}
