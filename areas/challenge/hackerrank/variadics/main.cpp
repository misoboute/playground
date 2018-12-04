// https://www.hackerrank.com/challenges/cpp-variadics/problem

#include <iostream>

#include <gtest/gtest.h>

#define SM_DISABLE_SMQDBG // Disable inclusion and use of qDebug()
#include "SMDebug.h"

// Enter your code for reversed_binary_value<bool...>()

struct RevBinVal
{
    template <int>
    static int Get()
    {
        return 0;
    }

    template <int, bool thisDigit, bool...otherDigits>
    static int Get()
    {
        return thisDigit + 2 * Get<0, otherDigits...>();
    }
};

template <bool...digits>
int reversed_binary_value()
{
    return RevBinVal::Get<0, digits...>();
}

template <int n, bool...digits>
struct CheckValues {
    static void check(int x, int y)
    {
        CheckValues<n - 1, 0, digits...>::check(x, y);
        CheckValues<n - 1, 1, digits...>::check(x, y);
    }
};

template <bool...digits>
struct CheckValues<0, digits...> {
    static void check(int x, int y)
    {
        int z = reversed_binary_value<digits...>();
        std::cout << (z + 64 * y == x);
    }
};

struct CppVariadicFixture : public testing::Test
{
    CppVariadicFixture()
    {
    }

    ~CppVariadicFixture()
    {
    }
};

TEST_F(CppVariadicFixture, T1)
{
    auto val = reversed_binary_value<true, true, true>();
    EXPECT_EQ(val, 7);
}

TEST_F(CppVariadicFixture, T2)
{
    auto val = reversed_binary_value<false, true, true>();
    EXPECT_EQ(val, 6);
}

TEST_F(CppVariadicFixture, T3)
{
    auto val = reversed_binary_value<false, true, true, false, false, true>();
    EXPECT_EQ(val, 70);
}

