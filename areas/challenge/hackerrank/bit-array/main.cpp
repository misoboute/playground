#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <unordered_set>
#include <vector>

#include <gtest/gtest.h>

#include "RandomGenerator.h"
#define SM_DISABLE_SMQDBG // Disable inclusion and use of qDebug()
#include "SMDebug.h"

using testing::Test;

int solution(uint32_t n, uint32_t s, uint32_t p, uint32_t q)
{
    std::unordered_set<uint32_t> seq;
    auto a = s;
    for (uint32_t i = 1; i <= n - 1 && seq.find(a) == seq.cend(); ++i)
    {
        seq.insert(a);
        a = (a * p + q) % (1 << 31);
    }
    seq.insert(a);
    return seq.size();
}

void solution(std::istream& cin, std::ostream& cout) 
{
    uint32_t n, s, p, q;
    cin >> n >> s >> p >> q;
    cout << solution(n, s, p, q);
}


struct BitArrayFixture : public Test
{
    BitArrayFixture()
    {
    }

    ~BitArrayFixture()
    {
    }

};

TEST_F(BitArrayFixture, Sample)
{
    std::istringstream cin("3 1 1 1");
    std::ostringstream cout;
    solution(cin, cout);
    EXPECT_EQ(cout.str(), "3");
}

TEST_F(BitArrayFixture, RandT1)
{
    uint32_t n = 19522407, s = 1269555274, p = 416176803, q = 496273252;
    //EXPECT_EQ(solution(n, s, p, q), 19522407);
}

TEST_F(BitArrayFixture, RandT2)
{
    uint32_t n = 47622805, s = 339837281, p = 1439023982, q = 1574682413;
    EXPECT_EQ(solution(n, s, p, q), 31);
}

TEST_F(BitArrayFixture, T1)
{
    smop::RandomGenerator rnd;
    auto n = rnd.Integer(uint32_t(1), uint32_t(100000000)),
        s = rnd.Integer(uint32_t(1), uint32_t(1 << 31)),
        p = rnd.Integer(uint32_t(1), uint32_t(1 << 31)),
        q = rnd.Integer(uint32_t(1), uint32_t(1 << 31));
    SMDW5(n, s, p, q, solution(n, s, p, q));
}
