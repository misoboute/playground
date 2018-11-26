
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>

#include <gtest/gtest.h>

using testing::Test;

const auto maxInt = std::numeric_limits<int>::max();
size_t called = 0, calledM = 0;

inline bool laneExists(const std::vector<int>& map, int seg, int lane)
{
    return map[seg] & (1 << lane);
}

int minChanges(const std::vector<int>& map, int curSeg, int curLane)
{
    if (++called == 1000000)
        calledM++, called = 0;

    if (curSeg >= map.size())
        return 0;

    auto straight = laneExists(map, curSeg, curLane) ?
        minChanges(map, curSeg + 1, curLane) : maxInt;
    if (curSeg == 0)
        return straight;

    auto left = curLane > 0 && laneExists(map, curSeg, curLane - 1) ?
        minChanges(map, curSeg + 1, curLane - 1) : maxInt;
    auto right = curLane < 7 && laneExists(map, curSeg, curLane + 1) ?
        minChanges(map, curSeg + 1, curLane + 1) : maxInt;

    if (left == right && right == maxInt)
        return straight;

    return std::min(std::min(left, right) + 1, straight);
}
// The recursive solution, although correct (and maybe more intuitive), is 
// of exponential complexity w.r.t map.size(). 
int solutionRecursive(const std::vector<int>& map)
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

// The iterative solution is of O(n) w.r.t map.size()
int solution(const std::vector<int>& map)
{
    std::vector< std::vector<int> > blackboard(
        2, std::vector<int>(8, 0));
    const auto maxInt = std::numeric_limits<int>::max();
    for (auto i = 0; i < map.size(); ++i)
    {
        auto& currentRow = blackboard[i % 2], prevRow = blackboard[(i + 1) % 2];
        for (auto j = 0; j < 8; ++j)
        {
            if (laneExists(map, i, j))
            {
                if (i == 0)
                    continue;

                auto straight = prevRow[j];
                auto left = j > 0 ? prevRow[j - 1] : maxInt;
                auto right = j < 7 ? prevRow[j + 1] : maxInt;
                if (left == maxInt) left--;
                if (right == maxInt) right--;
                currentRow[j] = std::min({ left + 1, right + 1, straight });
            }
            else
                currentRow[j] = maxInt;
        }
    }
    const auto& lastRow = blackboard[(map.size() - 1) % 2];
    auto minChanges = *std::min_element(lastRow.cbegin(), lastRow.cend());
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

    template <typename ListT>
    std::string listToStr(const ListT& list)
    {
        std::ostringstream str;
        auto itr = list.cbegin();
        str << "{ " << *itr++;
        while (itr != list.cend())
            str << ", " << *itr++;
        str << " };";
        return str.str();
    }

    std::string roadToStr(const std::vector<int>& road)
    {
        std::ostringstream str;
        for (auto j = 0; j < road.size(); ++j)
        {
            for (auto i = 0; i < 8; ++i)
                str << ((road[j] & (1 << i)) ? "#" : " ");
            str << "  :  " << road[j] << "  \t" << j << std::endl;
        }
        return str.str();
    }


    std::chrono::steady_clock::time_point started_;
};

TEST_F(TomTomFixture, T1)
{
    std::vector<int> map{ 7, 11, 10, 4 };
    std::cout << "map = " << listToStr(map) << std::endl << roadToStr(map);
    EXPECT_EQ(solution(std::move(map)), 1);
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

TEST_CHECKERED_MAP(10)  
TEST_CHECKERED_MAP(11)  
TEST_CHECKERED_MAP(12)  
TEST_CHECKERED_MAP(13)  
TEST_CHECKERED_MAP(14)  
TEST_CHECKERED_MAP(15)  
TEST_CHECKERED_MAP(16)  
TEST_CHECKERED_MAP(17)  
TEST_CHECKERED_MAP(18)  
TEST_CHECKERED_MAP(19)  
TEST_CHECKERED_MAP(20)  
TEST_CHECKERED_MAP(21)  
TEST_CHECKERED_MAP(22)  
TEST_CHECKERED_MAP(23)  
TEST_CHECKERED_MAP(24)  
TEST_CHECKERED_MAP(25)  
TEST_CHECKERED_MAP(26)  
TEST_CHECKERED_MAP(27)  
TEST_CHECKERED_MAP(28)  
TEST_CHECKERED_MAP(29)  
TEST_CHECKERED_MAP(30)  
TEST_CHECKERED_MAP(1000)

TEST_F(TomTomFixture, RandomRoad_10)
{
    std::vector<int> map{ 223, 123, 26, 178, 34, 231, 60, 119, 181, 191 };
    std::cout << "map = " << listToStr(map) << std::endl << roadToStr(map);
    EXPECT_EQ(solution(map), 1);
}

TEST_F(TomTomFixture, RandomRoad_58)
{
    std::vector<int> map{ 223, 123, 26, 178, 34, 231, 60, 119, 181, 191, 11, 36, 131, 36, 245, 38, 198, 196, 41, 206, 16, 178 , 80, 110, 171, 82, 20, 218, 47, 186, 100, 18, 24, 98, 205, 35, 169, 82, 92, 142, 65, 101, 17, 182, 232, 179, 179, 137, 110, 184, 69, 76, 186, 99, 170, 133, 6, 148, 4 };
    std::cout << "map = " << listToStr(map) << std::endl << roadToStr(map);
    EXPECT_EQ(solution(map), 25);
}

TEST_F(TomTomFixture, T9)
{
    std::vector<int> map{ 1, 3, 5, 9, 17, 33, 65 };
    for (int i = 0; i < 1000; i++)
    {
        map.push_back(129);
        map.push_back(130);
    }
    std::cout << "map = " << listToStr(map) << std::endl << roadToStr(map);
    EXPECT_EQ(solution(map), 7);
}

