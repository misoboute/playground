#include <sstream>
#include <stack>

#include <gtest/gtest.h>

using testing::Test;

int solution(std::string &S)
{
    const int MaxNumber = (1 << 20) - 1;
    std::istringstream inputStream(S);
    std::stack<int> numStack;
    while (inputStream.good())
    {
        std::string singleInput;
        inputStream >> singleInput;
        if (singleInput == "+")
        {
            if (numStack.size() < 2)
            {
                return -1;
            }
            auto num1 = numStack.top();
            numStack.pop();
            auto num2 = numStack.top();
            numStack.pop();
            auto sum = num1 + num2;
            if (sum > MaxNumber)
            {
                return -1;
            }
            numStack.push(sum);
        }
        else if (singleInput == "-")
        {
            if (numStack.size() < 2)
            {
                return -1;
            }
            auto num1 = numStack.top();
            numStack.pop();
            auto num2 = numStack.top();
            numStack.pop();
            auto sum = num1 - num2;
            if (sum < 0)
            {
                return -1;
            }
            numStack.push(sum);
        }
        else if (singleInput == "DUP")
        {
            if (numStack.size() < 1)
            {
                return -1;
            }
            numStack.push(numStack.top());
        }
        else if (singleInput == "POP")
        {
            if (numStack.size() < 1)
            {
                return -1;
            }
            numStack.pop();
        }
        else
        {
            std::istringstream numParser(singleInput);
            int newNum = -1;
            numParser >> newNum;
            if (newNum == -1)
            {
                continue;
            }
            numStack.push(newNum);
        }
    }
    return numStack.empty() ? -1 : numStack.top();
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
    EXPECT_EQ(solution(std::string("13 DUP 4 POP 5 DUP + DUP + -")), 7);
}

TEST_F(CyberComChal1Fixture, T2)
{
    EXPECT_EQ(solution(std::string("5 6 + -")), -1);
}

TEST_F(CyberComChal1Fixture, T3)
{
    EXPECT_EQ(solution(std::string("3 DUP 5 - -")), -1);
}

TEST_F(CyberComChal1Fixture, T4)
{
    EXPECT_EQ(solution(std::string("POP")), -1);
}

TEST_F(CyberComChal1Fixture, T5)
{
    EXPECT_EQ(solution(std::string("1000 2000 3000 POP POP POP")), -1);
}

TEST_F(CyberComChal1Fixture, T6)
{
    EXPECT_EQ(solution(std::string("1000 2000 3000 POP POP POP POP +")), -1);
}

TEST_F(CyberComChal1Fixture, T7)
{
    EXPECT_EQ(solution(std::string("1000 2000 DUP DUP + DUP 3000 -")), -1);
}

TEST_F(CyberComChal1Fixture, T8)
{
    EXPECT_EQ(solution(std::string("1000 2000 DUP DUP + DUP 5500 -")), 1500);
}

TEST_F(CyberComChal1Fixture, T9)
{
    EXPECT_EQ(solution(std::string("1000 2000 DUP DUP + DUP 5500 - 300 3000 POP +")), 1800);
}

TEST_F(CyberComChal1Fixture, T10)
{
    EXPECT_EQ(solution(std::string("1000000 DUP +")), -1);
}

TEST_F(CyberComChal1Fixture, T11)
{
    EXPECT_EQ(solution(std::string("+")), -1);
}

TEST_F(CyberComChal1Fixture, T12)
{
    EXPECT_EQ(solution(std::string("-")), -1);
}

TEST_F(CyberComChal1Fixture, T13)
{
    EXPECT_EQ(solution(std::string("DUP")), -1);
}

TEST_F(CyberComChal1Fixture, T14)
{
    EXPECT_EQ(solution(std::string("1047575 1001 +")), -1);
}

TEST_F(CyberComChal1Fixture, T15)
{
    EXPECT_EQ(solution(std::string("1038575 10000 +")), 1048575);
}

TEST_F(CyberComChal1Fixture, T16)
{
    EXPECT_EQ(solution(std::string("1038575 1038574 -")), -1);
}

TEST_F(CyberComChal1Fixture, T17)
{
    EXPECT_EQ(solution(std::string("1038575 1038573 -")), -1);
}

TEST_F(CyberComChal1Fixture, T18)
{
    EXPECT_EQ(solution(std::string("3 DUP DUP DUP -")), 0);
}

TEST_F(CyberComChal1Fixture, T19)
{
    EXPECT_EQ(solution(std::string("3 DUP DUP DUP - -")), -1);
}

TEST_F(CyberComChal1Fixture, T20)
{
    EXPECT_EQ(solution(std::string("3 -")), -1);
}

TEST_F(CyberComChal1Fixture, T21)
{
    EXPECT_EQ(solution(std::string("30 +")), -1);
}

TEST_F(CyberComChal1Fixture, T22)
{
    EXPECT_EQ(solution(std::string("4 5 - POP DUP 3")), -1);
}

TEST_F(CyberComChal1Fixture, T23)
{
    EXPECT_EQ(solution(std::string("")), -1);
}

TEST_F(CyberComChal1Fixture, T24)
{
    EXPECT_EQ(solution(std::string("1 ")), 1);
}
