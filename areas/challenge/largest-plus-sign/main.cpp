// LeetCode 764. Largest Plus Sign (Medium)

// You are given an integer n. You have an n x n binary grid grid with all values initially 1's except for some indices given in the array mines. The ith element of the array mines is defined as mines[i] = [xi, yi] where grid[xi][yi] == 0.

// Return the order of the largest axis-aligned plus sign of 1's contained in grid. If there is none, return 0.

// An axis-aligned plus sign of 1's of order k has some center grid[r][c] == 1 along with four arms of length k - 1 going up, down, left, and right, and made of 1's. Note that there could be 0's or 1's beyond the arms of the plus sign, only the relevant area of the plus sign is checked for 1's.

// Example 1:
// Input: n = 5, mines = [[4,2]]
// Output: 2
// Explanation: In the above grid, the largest plus sign can only be of order 2. One of them is shown.

// Example 2:
// Input: n = 1, mines = [[0,0]]
// Output: 0
// Explanation: There is no plus sign, so return 0.

// Constraints:
// 1 <= n <= 500
// 1 <= mines.length <= 5000
// 0 <= xi, yi < n
// All the pairs (xi, yi) are unique.

#include <cstdint>
#include <set>
#include <vector>

class Solution
{
public:
    int orderOfLargestPlusSign(int n, std::vector< std::vector<int> >& mines)
    {
        auto mineCols = std::vector< std::set<CellIndex> >(n);
        auto mineRows = std::vector< std::set<CellIndex> >(n);
        for (const auto& coords : mines)
        {
            mineRows[coords[0]].insert(coords[1]);
            mineCols[coords[1]].insert(coords[0]);
        }
        clearVector(std::move(mines));
        
        for (auto ord = n / 2 + n % 2; ord > 0; --ord)
        {
            const auto upTo = n - ord + 1;
            for (CellIndex col = ord - 1; col < upTo; ++col)
            {
                for (CellIndex row = ord - 1; row < upTo; ++row)
                {
                    auto vertFind = mineCols[col].lower_bound(row - ord + 1);
                    auto horizFind = mineRows[row].lower_bound(col - ord + 1);
                    auto noVertHit = vertFind == mineCols[col].cend() || (*vertFind) >= row + ord;
                    auto noHorizHit = horizFind == mineRows[row].cend() || (*horizFind) >= col + ord;
                    if (noVertHit && noHorizHit)
                    {
                        return ord;
                    }
                }
            }
        }
        return 0;
    }

private:
    using CellIndex = int16_t;

    void clearVector(std::vector< std::vector<int> >&& mines)
    {
        mines.clear();
    }
};

#include <gtest/gtest.h>

using testing::Test;

struct LargestPlusSignFixture : public Test
{
    LargestPlusSignFixture()
    {

    }

    ~LargestPlusSignFixture()
    {
    }

    Solution m_Sol;
};

TEST_F(LargestPlusSignFixture, T1)
{
    std::vector< std::vector<int> > mines{ { 4, 2 } };
    EXPECT_EQ(m_Sol.orderOfLargestPlusSign(5, mines), 2);
}

TEST_F(LargestPlusSignFixture, T2)
{
    std::vector< std::vector<int> > mines{ { 0, 0 } };
    EXPECT_EQ(m_Sol.orderOfLargestPlusSign(1, mines), 0);
}

TEST_F(LargestPlusSignFixture, T3)
{
    std::vector< std::vector<int> > mines{ { 2, 4 }, { 4, 2 } };
    EXPECT_EQ(m_Sol.orderOfLargestPlusSign(6, mines), 3);
}

TEST_F(LargestPlusSignFixture, T4)
{
    std::vector< std::vector<int> > mines{ { 2, 4 }, { 4, 2 } };
    EXPECT_EQ(m_Sol.orderOfLargestPlusSign(7, mines), 4);
}

TEST_F(LargestPlusSignFixture, T5)
{
    std::vector< std::vector<int> > mines{ { 0, 0 }, { 0, 1 }, { 1, 0 } };
    EXPECT_EQ(m_Sol.orderOfLargestPlusSign(2, mines), 1);
}
