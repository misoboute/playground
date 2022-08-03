// sig

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> balances, vector<string> requests)
{
    for (size_t k = 0; k < requests.size(); ++k)
    {
        istringstream reqIStr(requests[k]);
        string reqType;
        int i = 0, sum = 0;
        reqIStr >> reqType;
        if (reqType == "transfer")
        {
            int j = 0;
            reqIStr >> i >> j >> sum;
            if (i > balances.size() || j > balances.size() || balances[i - 1] < sum)
            {
                return { -static_cast<int>(k + 1) };
            }
            balances[j - 1] += sum;
            balances[i - 1] -= sum;
        }
        else if (reqType == "deposit")
        {
            reqIStr >> i >> sum;
            if (i > balances.size())
            {
                return { -static_cast<int>(k + 1) };
            }
            balances[i - 1] += sum;
        }
        else if (reqType == "withdraw")
        {
            reqIStr >> i >> sum;
            if (i > balances.size() || balances[i - 1] < sum)
            {
                return { -static_cast<int>(k + 1) };
            }
            balances[i - 1] -= sum;
        }
        else 
        {
            return { -static_cast<int>(k + 1) };
        }
    }
    return balances;
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::Test;
using testing::ElementsAre;

struct SigFixture : public Test
{
    SigFixture()
    {
    }

    ~SigFixture()
    {
    }
};

TEST_F(SigFixture, T1)
{
    EXPECT_THAT(solution(
        { 100, 50, 0, 10, 30}, 
        { 
            "transfer 2 3 25", 
            "deposit 3 25",
            "transfer 3 4 20",
            "withdraw 1 30",
            "transfer 5 2 20",
            "deposit 5 30",
            "withdraw 1 20",
            "transfer 2 4 15",
            "withdraw 2 15",
            "deposit 1 40",
            "transfer 5 1 10",
            }),
        ElementsAre(100, 15, 30, 45, 30));
}

TEST_F(SigFixture, T2)
{
    EXPECT_THAT(solution(
        { 100, 50, 0, 10, 30}, 
        { 
            "transfer 2 3 25", 
            "deposit 3 25",
            "transfer 3 4 20",
            "withdraw 1 30",
            "transfer 5 2 20",
            "deposit 5 30",
            "withdraw 1 20",
            "transfer 6 4 15",
            "withdraw 2 15",
            "deposit 1 40",
            "transfer 5 1 10",
            }),
        ElementsAre(-8));
}

TEST_F(SigFixture, T3)
{
    EXPECT_THAT(solution(
        { 100, 50, 0, 10, 30}, 
        { 
            "transfer 2 3 25", 
            "deposit 3 25",
            "transfer 3 4 20",
            "withdraw 1 30",
            "transfer 5 2 20",
            "deposit 5 30",
            "withdraw 1 20",
            "transfer 2 6 15",
            "withdraw 2 15",
            "deposit 1 40",
            "transfer 5 1 10",
            }),
        ElementsAre(-8));
}

TEST_F(SigFixture, T4)
{
    EXPECT_THAT(solution(
        { 100, 50, 0, 10, 30}, 
        { 
            "transfer 2 3 25", 
            "deposit 3 25",
            "transfer 3 4 20",
            "withdraw 1 30",
            "transfer 5 2 20",
            "deposit 5 30",
            "withdraw 1 20",
            "transfer 2 4 15",
            "withdraw 2 15",
            "deposit 1 40",
            "transfer 5 1 10",
            "withdraw 3 31",
            "transfer 3 4 20",
            "withdraw 1 30",
            "transfer 5 2 20",
            }),
        ElementsAre(-12));
}

TEST_F(SigFixture, T5)
{
    EXPECT_THAT(solution(
        { 100, 50, 0, 10, 30}, 
        { 
            "transfer 2 3 25", 
            "deposit 3 25",
            "transfer 3 4 20",
            "withdraw 1 30",
            "transfer 5 2 20",
            "deposit 5 30",
            "withdraw 1 20",
            "transfer 2 4 15",
            "withdraw 2 15",
            "deposit 1 40",
            "transfer 5 1 10",
            "transfer 1 5 101",
            "transfer 3 4 20",
            "withdraw 1 30",
            "transfer 5 2 20",
            }),
        ElementsAre(-12));
}

TEST_F(SigFixture, T6)
{
}

TEST_F(SigFixture, T7)
{
}

TEST_F(SigFixture, T8)
{
}

TEST_F(SigFixture, T9)
{
}

TEST_F(SigFixture, T10)
{
}

TEST_F(SigFixture, T11)
{
}

TEST_F(SigFixture, T12)
{
}

TEST_F(SigFixture, T13)
{
}
