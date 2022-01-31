// LeetCode 413. Arithmetic Slices (Medium)

// An integer array is called arithmetic if it consists of at least three elements and if the difference between any two consecutive elements is the same.

// For example, [1,3,5,7,9], [7,7,7,7], and [3,-1,-5,-9] are arithmetic sequences.
// Given an integer array nums, return the number of arithmetic subarrays of nums.

// A subarray is a contiguous subsequence of the array.

// Example 1:
// Input: nums = [1,2,3,4]
// Output: 3
// Explanation: We have 3 arithmetic slices in nums: [1, 2, 3], [2, 3, 4] and [1,2,3,4] itself.

// Example 2:
// Input: nums = [1]
// Output: 0
 
// Constraints:
// 1 <= nums.length <= 5000
// -1000 <= nums[i] <= 1000

#include <limits>
#include <numeric>
#include <vector>

class Solution {
public:
    int numberOfArithmeticSlices(std::vector<int>& nums)
    {
        if (nums.size() < 3)
            return 0;

        auto total = 0;
        auto curSliceSize = 0;
        auto lastDiff = nums[1] - nums[0];
        for (auto i = 2; i < nums.size(); ++i)
        {
            auto curDiff = nums[i] - nums[i - 1];
            if (curDiff == lastDiff)
                ++curSliceSize;
            else
            {
                total += curSliceSize * (curSliceSize + 1) / 2;
                curSliceSize = 0;
            }
            lastDiff = curDiff;
        }
        if (curSliceSize > 0)
        {
            total += curSliceSize * (curSliceSize + 1) / 2;
        }
        return total;
    }
};

#include <gtest/gtest.h>

using testing::Test;

struct ArithmeticSlicesFixture : public Test
{
    ArithmeticSlicesFixture()
    {
    }

    ~ArithmeticSlicesFixture()
    {
    }

    Solution m_Sol;
};

TEST_F(ArithmeticSlicesFixture, T1)
{
    std::vector<int> nums{ 1 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 0);
}

TEST_F(ArithmeticSlicesFixture, T2)
{
    std::vector<int> nums{ 1, 2, 3, 4 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 3);
}

TEST_F(ArithmeticSlicesFixture, T3)
{
    std::vector<int> nums{ 1, 5, 9, 13 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 3);
}

TEST_F(ArithmeticSlicesFixture, T4)
{
    std::vector<int> nums{ 13, 10, 7 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 1);
}

TEST_F(ArithmeticSlicesFixture, T5)
{
    std::vector<int> nums{ 7, 9, 11, 13, 15 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 6);
}

TEST_F(ArithmeticSlicesFixture, T6)
{
    std::vector<int> nums{ 13, 15, 5, 3, 6 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 0);
}

TEST_F(ArithmeticSlicesFixture, T7)
{
    std::vector<int> nums{ 13, 15, 5, 3, 6, 9, 12, 15, 18, -1, -4 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 10);
}

TEST_F(ArithmeticSlicesFixture, T8)
{
    std::vector<int> nums{ 15, 18, -1, -4, -7, -2, 0 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 1);
}

TEST_F(ArithmeticSlicesFixture, T9)
{
    std::vector<int> nums{ 1, 5, 9, 13, 10, 7, 9, 11, 13, 15, 5, 3, 6, 9, 12, 15, 18, -1, -4, -7, -2, 0, 2, 4, 6 };
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 27);
}

TEST_F(ArithmeticSlicesFixture, T10)
{
    std::vector<int> nums{ 10, 20, 30, 40, 31, 39, 32, 37, 33, 36, 34, 35, 30, 25};
    EXPECT_EQ(m_Sol.numberOfArithmeticSlices(nums), 4);
}
