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

int solMain(std::istream& cin, std::ostream& cout)
{
    int t; cin >> t;

    for (int i = 0; i != t; ++i) {
        int x, y;
        cin >> x >> y;
        CheckValues<6>::check(x, y);
        cout << "\n";
    }

    return 0;
}

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

TEST_F(CppVariadicFixture, Sample)
{
    std::istringstream cin("2\n65 1\n10 0");
    std::ostringstream cout;
    solMain(cin, cout);
    EXPECT_EQ(cout.str(),
        "0100000000000000000000000000000000000000000000000000000000000000\n"
        "0000000000100000000000000000000000000000000000000000000000000000\n");
}
