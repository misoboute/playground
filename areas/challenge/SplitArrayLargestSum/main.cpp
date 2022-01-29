// 410. Split Array Largest Sum (Hard)

// Given an array nums which consists of non-negative integers and an integer m, you can split the array into m non-empty continuous subarrays.

// Write an algorithm to minimize the largest sum among these m subarrays.

// Example 1:

// Input: nums = [7,2,5,10,8], m = 2
// Output: 18

// Explanation:
// There are four ways to split nums into two subarrays.
// The best way is to split it into [7,2,5] and [10,8],
// where the largest sum among the two subarrays is only 18.

// Example 2:

// Input: nums = [1,2,3,4,5], m = 2
// Output: 9
// Example 3:

// Input: nums = [1,4,4], m = 3
// Output: 4
 
// Constraints:

// 1 <= nums.length <= 1000
// 0 <= nums[i] <= 10^6
// 1 <= m <= min(50, nums.length)

#include <limits>
#include <numeric>
#include <vector>

// Creates and maintains an internal lookup table for the sums of all the 
// possible sub-arrays of a given std::vector so that the sum of any sub-array
// could be retrieved in constant time. During the construction, it creates 
// the lookup table and computes all the possible sums.
class PartialSumsLookup
{
public:
    PartialSumsLookup(const std::vector<int>& nums)
    : m_Sums(nums.size())
    {
        for (int i = 0; i < m_Sums.size(); ++i)
        {
            m_Sums[i].resize(nums.size() - i);
            m_Sums[i][0] = nums[i];
        }
        for (int loHiDist = 1; loHiDist < nums.size(); ++loHiDist)
        {
            At(0, loHiDist) = At(0, loHiDist - 1) + nums[loHiDist];
            auto lastSum = At(0, loHiDist);
            for (int low = 1; low < nums.size() - loHiDist; ++low)
            {
                lastSum = 
                    At(low, low + loHiDist) = 
                        lastSum + nums[low + loHiDist] - nums[low - 1];
            }
        }
    }

    // Returns the sum of the range starting from the ith element up to
    // (and including) the jth element.
    inline int operator()(int i, int j)
    {
        return At(i, j);
    }

private:
    inline int& At(int i, int j)
    {
        return m_Sums[i][j - i];
    }

    std::vector< std::vector<int> > m_Sums;
};

class Solution {
public:
    int splitArray(std::vector<int>& nums, int m)
    {
        const auto size = nums.size();
        if (m < 2)
            return std::accumulate(nums.cbegin(), nums.cend(), 0);

        PartialSumsLookup sum(nums);
        m_SplitSubArray.resize(m - 1);
        m_SplitSubArray[0].resize(size - m + 1);
        for (int i = 0; i < m_SplitSubArray[0].size(); ++i)
            m_SplitSubArray[0][i] = sum(m + i - 1, size - 1);
        // mp_1 : mPrime - 1
        for (int mp_1 = 1; mp_1 < m - 1; ++mp_1)
        {
            m_SplitSubArray[mp_1].resize(size - m + 1);
            // sai : sub-array index to split to mp_1 parts
            for (int sai = 0; sai < m_SplitSubArray[mp_1].size(); ++sai)
            {
                // sub-array starts at index of the main array
                // n: 15, m: 5, mp_1: 1, sai: 0, start at index 3
                // n: 15, m: 5, mp_1: 1, sai: 1, start at index 4
                // n: 15, m: 5, mp_1: 2, sai: 0, start at index 2
                // n: 15, m: 5, mp_1: 2, sai: 0, start at index 3
                // sub-array absolute start index: m - mp_1 + sai - 1
                const auto saasi = m - mp_1 + sai - 1;
                // sub-array last+1 split index: 
                const auto salsi = size - m - sai + 1;
                auto minSplitSum = std::numeric_limits<int>::max();
                for (int i = 0; i < salsi; ++i)
                {
                    auto splitAtI = 
                        std::max(sum(saasi, saasi + i), m_SplitSubArray[mp_1 - 1][i + sai]);
                    if (splitAtI < minSplitSum)
                        minSplitSum = splitAtI;
                }
                m_SplitSubArray[mp_1][sai] = minSplitSum;
            }
        }
        auto minSplitSum = std::numeric_limits<int>::max();
        for (int i = 0; i < size - m + 1; ++i)
        {
            auto splitAtI = 
                std::max(sum(0, i), m_SplitSubArray[m - 2][i]);
            if (splitAtI < minSplitSum)
                minSplitSum = splitAtI;
        }
        return minSplitSum;
    }

private:
    std::vector< std::vector<int> > m_SplitSubArray;
};

#include <gtest/gtest.h>

using testing::Test;

struct PartialSumsLookupFixture : public Test
{
    PartialSumsLookupFixture()
    {
    }

    ~PartialSumsLookupFixture()
    {
    }

};

struct SplitArrayLargestSumFixture : public Test
{
    SplitArrayLargestSumFixture()
    {
    }

    ~SplitArrayLargestSumFixture()
    {
    }

    Solution m_Sol;
};

TEST_F(PartialSumsLookupFixture, PT1)
{
    std::vector<int> nums{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
    PartialSumsLookup sum(nums);
    EXPECT_EQ(sum(0, 0), 2);
    EXPECT_EQ(sum(9, 9), 20);
    EXPECT_EQ(sum(0, 1), 6);
    EXPECT_EQ(sum(1, 2), 10);
    EXPECT_EQ(sum(8, 9), 38);
    EXPECT_EQ(sum(0, 2), 12);
    EXPECT_EQ(sum(0, 5), 42);
    EXPECT_EQ(sum(5, 9), 80);
    EXPECT_EQ(sum(3, 7), 60);
    EXPECT_EQ(sum(0, 9), 110);
}

TEST_F(PartialSumsLookupFixture, PT2)
{
    std::vector<int> nums(1000);
    for (auto i = 0; i < nums.size(); ++i)
        nums[i] = 3 * i;
    PartialSumsLookup sum(nums);
    auto calcSum = [](auto i, auto j)
    {
        return 3 * (j * (j + 1) - i * (i - 1)) / 2;
    };
    EXPECT_EQ(sum(0, 0), 0);
    EXPECT_EQ(sum(9, 9), 27);
    EXPECT_EQ(sum(0, 1), 3);
    EXPECT_EQ(sum(1, 2), 9);
    EXPECT_EQ(sum(8, 9), 51);
    EXPECT_EQ(sum(0, 2), 9);
    EXPECT_EQ(sum(0, 5), 45);
    EXPECT_EQ(sum(5, 9), 105);
    EXPECT_EQ(sum(3, 7), 75);
    EXPECT_EQ(sum(0, 9), 135);
    EXPECT_EQ(sum(0, 9), calcSum(0, 9));
    EXPECT_EQ(sum(40, 80), calcSum(40, 80));
    EXPECT_EQ(sum(150, 830), calcSum(150, 830));
    EXPECT_EQ(sum(212, 999), calcSum(212, 999));
    EXPECT_EQ(sum(0, 999), calcSum(0, 999));
}

TEST_F(SplitArrayLargestSumFixture, T1)
{
    std::vector<int> nums{ 7, 2, 5, 10, 8 };
    EXPECT_EQ(m_Sol.splitArray(nums, 2), 18);
}

TEST_F(SplitArrayLargestSumFixture, T2)
{
    std::vector<int> nums{ 1, 2, 3, 4, 5 };
    EXPECT_EQ(m_Sol.splitArray(nums, 2), 9);
}

TEST_F(SplitArrayLargestSumFixture, T3)
{
    std::vector<int> nums{ 1, 4, 4 };
    EXPECT_EQ(m_Sol.splitArray(nums, 3), 4);
}

TEST_F(SplitArrayLargestSumFixture, T4)
{
    std::vector<int> nums{ 1, 2, 3, 4, 5 };
    EXPECT_EQ(m_Sol.splitArray(nums, 3), 6);
}

TEST_F(SplitArrayLargestSumFixture, T5)
{
    std::vector<int> nums(15);
    for (auto i = 0; i < nums.size(); ++i)
        nums[i] = i - 2;
    EXPECT_EQ(m_Sol.splitArray(nums, 10), 12);
}

TEST_F(SplitArrayLargestSumFixture, T6)
{
    std::vector<int> nums{ 7,2,5,10,8,22,16,11,14,56,11,24,53,21,56,32,4 };
    EXPECT_EQ(m_Sol.splitArray(nums, 5), 81);
}

TEST_F(SplitArrayLargestSumFixture, T7)
{
    std::vector<int> nums{ 7 };
    EXPECT_EQ(m_Sol.splitArray(nums, 1), 7);
}

TEST_F(SplitArrayLargestSumFixture, T8)
{
    std::vector<int> nums{ 7, 10 };
    EXPECT_EQ(m_Sol.splitArray(nums, 2), 10);
}

TEST_F(SplitArrayLargestSumFixture, T9)
{
    std::vector<int> nums{ 7, 10 };
    EXPECT_EQ(m_Sol.splitArray(nums, 1), 17);
}
