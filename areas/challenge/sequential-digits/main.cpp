// An integer has sequential digits if and only if each digit in the number is one more than the previous digit.

// Return a sorted list of all the integers in the range [low, high] inclusive that have sequential digits.

// Example 1:
// Input: low = 100, high = 300
// Output: [123,234]

// Example 2:
// Input: low = 1000, high = 13000
// Output: [1234,2345,3456,4567,5678,6789,12345]
 
// Constraints:
// 10 <= low <= high <= 10^9

#include <vector>

class Solution {
public:
    std::vector<int> sequentialDigits(int low, int high) {
        std::vector<int> out;
        auto [lowFirstDig, lowDigs] = getFirstDigitAndNumDigits(low);
        auto [highFirstDig, highDigs] = getFirstDigitAndNumDigits(high);
        auto sameDigs = lowDigs == highDigs;
        if (sameDigs)
        {
            getNums(out, low, high, lowDigs, lowFirstDig, highFirstDig);
        }
        else
        {
            getNums(out, low, oneZeros(lowDigs), lowDigs, lowFirstDig, 10 - lowDigs);
        }
        for (auto i = lowDigs + 1; i < highDigs; ++i)
        {
            getNums(out, oneZeros(i - 1), oneZeros(i), i, 1, 10 - i);
        }
        if (!sameDigs)
        {
            getNums(out, oneZeros(highDigs - 1), high, highDigs, 1, highFirstDig);
        }
        return out;
    }

private:
    std::pair<int, int> getFirstDigitAndNumDigits(int num)
    {
        int numDigits = 1;
        auto firstDig = num;
        while (firstDig > 9)
        {
            ++numDigits;
            firstDig /= 10;
        }
        return { firstDig, numDigits };
    }

    void getNums(std::vector<int>& out, int lowNum, int highNum, int numDigs, int minFirstDig, int maxFirstDig)
    {
        for (auto i = minFirstDig; i <= std::min(maxFirstDig, 10 - numDigs); ++i)
        {
            auto num = makeNum(i, numDigs);
            if (num >= lowNum && num <= highNum)
                out.push_back(num);
        }
    }

    int makeNum(int firstDig, int numDigs)
    {
        auto num = firstDig;
        for (auto i = firstDig + 1; i < std::min(10, firstDig + numDigs); ++i)
        {
            num *= 10;
            num += i;
        }
        return num;
    }

    int oneZeros(int n)
    {
        auto out = 1; 
        for (auto i = 0; i < n; ++i)
            out *= 10;
        return out;
    }

};

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::ElementsAre;
using testing::Test;

struct SequentialDigitsFixture : public Test
{
    SequentialDigitsFixture()
    {
    }

    ~SequentialDigitsFixture()
    {
    }

    Solution m_Sol;
};

TEST_F(SequentialDigitsFixture, T1)
{
    EXPECT_THAT(m_Sol.sequentialDigits(100, 300), ElementsAre(123, 234));
}

TEST_F(SequentialDigitsFixture, T2)
{
    EXPECT_THAT(m_Sol.sequentialDigits(1000, 13000), 
        ElementsAre(1234, 2345, 3456, 4567, 5678, 6789, 12345));
}

TEST_F(SequentialDigitsFixture, T3)
{
    EXPECT_THAT(m_Sol.sequentialDigits(14, 41), ElementsAre(23, 34));
}

TEST_F(SequentialDigitsFixture, T4)
{
    EXPECT_THAT(m_Sol.sequentialDigits(12, 45), ElementsAre(12, 23, 34, 45));
}

TEST_F(SequentialDigitsFixture, T5)
{
    EXPECT_THAT(m_Sol.sequentialDigits(14, 4368), 
        ElementsAre(23, 34, 45, 56, 67, 78, 89, 123, 234, 345, 456, 567, 678, 789, 1234, 2345, 3456));
}
