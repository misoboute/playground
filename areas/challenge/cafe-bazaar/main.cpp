#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

using testing::Test;

std::string InsertParens(
    const std::string& str,
    const std::vector< std::pair<size_t, size_t> >& parenPairs)
{
    std::map<size_t, std::pair<int, int> > parensMap;
    for (const auto& p : parenPairs)
    {
        ++parensMap[p.first].second;
        ++parensMap[p.second].first;
    }
    size_t lastIdx = 0;
    std::ostringstream sout;
    for (const auto& paren : parensMap)
    {
        sout << str.substr(lastIdx, paren.first - lastIdx);
        for (auto i = 0; i < paren.second.first; ++i)
        {
            sout << ")";
        }
        for (auto i = 0; i < paren.second.second; ++i)
        {
            sout << "(";
        }
        lastIdx = paren.first;
    }
    sout << str.substr(lastIdx);
    return sout.str();
}

struct CafeBazaarFixture : public Test
{
    CafeBazaarFixture()
    {
    }

    ~CafeBazaarFixture()
    {
    }
};

TEST_F(CafeBazaarFixture, T1)
{
    const auto str = "Hello, World!";
    const auto parenPairs = std::vector< std::pair<size_t, size_t> > {
        { 1, 2 },
        { 3, 5 }
    };
    EXPECT_EQ(InsertParens(str, parenPairs), "H(e)l(lo), World!");
}

TEST_F(CafeBazaarFixture, T2)
{
    const auto str = "Hello, World! ";
    const auto parenPairs = std::vector< std::pair<size_t, size_t> > {
        { 1, 5 },
        { 2, 3 }
    };
    EXPECT_EQ(InsertParens(str, parenPairs), "H(e(l)lo), World! ");
}

TEST_F(CafeBazaarFixture, T3)
{
    const auto str = "Hello, World!";
    const auto parenPairs = std::vector< std::pair<size_t, size_t> > {
        { 1, 2 },
        { 1, 2 }
    };
    EXPECT_EQ(InsertParens(str, parenPairs), "H((e))llo, World!");
}

TEST_F(CafeBazaarFixture, T4)
{
    const auto str = "Hello, World!";
    const auto parenPairs = std::vector< std::pair<size_t, size_t> > {
        { 0, 13 },
        { 0, 12 },
        { 12, 13 },
        { 1, 12 }
    };
    EXPECT_EQ(InsertParens(str, parenPairs), "((H(ello, World))(!))");
}
