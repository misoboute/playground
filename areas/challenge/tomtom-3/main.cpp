#include <iostream>

#include <algorithm>
#include <chrono>
#include <limits>

#include <gtest/gtest.h>

using testing::Test;

const auto maxInt = std::numeric_limits<int>::max();
size_t called = 0, calledM = 0;

int minChanges(const std::vector<int>& map, int curSeg, int curLane)
{
    if (++called == 1000000)
        calledM++, called = 0;

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
        called = calledM = 0;
        started_ = std::chrono::steady_clock::now();
    }

    ~TomTomFixture()
    {
        auto took = std::chrono::steady_clock::now() - started_;
        //std::cout << "Took " <<
        //    std::chrono::duration_cast<std::chrono::milliseconds>(took).count() 
        //    << "ms and called minChanges " << calledM << " million times." << 
        //    std::endl;
    }

    std::vector<int> CreateCheckeredMap(int length)
    {
        // 01010101
        // 10101010
        // 01010101
        // 10101010
        // 01010101
        // 10101010
        // ...
        std::vector<int> map(length);
        for (auto i = 0; i < map.size(); ++i)
            map[i] = 0x55 << (i % 2);
        return map;
    }

    std::chrono::steady_clock::time_point started_;
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
    // 00000001
    // 00000010
    // 00000100
    // 00001000
    // 00010000
    // 00100000
    // 01000000
    // 10000000
    // 01000000
    // 00100000
    // 00010000
    // 00001000
    // 00000100
    // 00000010
    // 00000001
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

#define TEST_CHECKERED_MAP(LENGTH) \
TEST_F(TomTomFixture, Checkered_##LENGTH) \
{ \
    EXPECT_EQ(solution(CreateCheckeredMap(LENGTH)), LENGTH - 1); \
} 

TEST_CHECKERED_MAP(10)  // (1 ms)
TEST_CHECKERED_MAP(11)  // (1 ms)
TEST_CHECKERED_MAP(12)  // (1 ms)
TEST_CHECKERED_MAP(13)  // (3 ms)
TEST_CHECKERED_MAP(14)  // (6 ms)
TEST_CHECKERED_MAP(15)  // (11 ms)
TEST_CHECKERED_MAP(16)  // (22 ms)
TEST_CHECKERED_MAP(17)  // (39 ms)
TEST_CHECKERED_MAP(18)  // (75 ms)
TEST_CHECKERED_MAP(19)  // (160 ms)
TEST_CHECKERED_MAP(20)  // (270 ms)
TEST_CHECKERED_MAP(21)  // (484 ms)
TEST_CHECKERED_MAP(22)  // (850 ms)
TEST_CHECKERED_MAP(23)  // (1595 ms)
TEST_CHECKERED_MAP(24)  // (2957 ms)
TEST_CHECKERED_MAP(25)  // (5577 ms)
TEST_CHECKERED_MAP(26)  // (10459 ms)
TEST_CHECKERED_MAP(27)  // (19869 ms)
TEST_CHECKERED_MAP(28)  // (37374 ms)
TEST_CHECKERED_MAP(29)  // (70691 ms)
TEST_CHECKERED_MAP(30)  // (131839 ms)

// The worst case complexity (a fully checkered road) is exponential w.r.t
// road length.
