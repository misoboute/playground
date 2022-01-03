#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);



/*
 * Complete the 'findValidDiscountCoupons' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts STRING_ARRAY discounts as parameter.
 */ // daabbd
bool isValid(const string& coup, size_t begin, size_t end);
bool trySplitting(const string& coup, size_t begin, size_t end);

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

using CacheT = std::unordered_map< std::pair<size_t, size_t>, bool, pair_hash>;

auto g_cache = CacheT(1);

void ResetCache(size_t strSize)
{
    g_cache = CacheT(strSize * strSize / 1024);
}

bool trySplitting(const string& coup, size_t begin, size_t end)
{
    if (end - begin < 4)
        return false;

    for(size_t i = begin + 2; i <= end - 2; i += 2)
    {
        if (isValid(coup, begin, i) && isValid(coup, i, end))
            return true;
    }
    return false;
}

bool isValid(const string& coup, size_t begin, size_t end)
{
    auto fromCache = g_cache.find({ begin, end });
    if (fromCache != g_cache.cend())
        return fromCache->second;
    if ((end - begin) % 2 != 0)
        return g_cache[{begin, end}] = false;
    if (trySplitting(coup, begin, end))
        return g_cache[{begin, end}] = true;
    while (end > begin && coup[begin] == coup[end - 1])
        ++begin, --end;
    if (begin >= end)
        return g_cache[{begin, end}] = true;
    return g_cache[{begin, end}] = trySplitting(coup, begin, end);
}

bool isValid(const string& coup)
{
    ResetCache(coup.size());
    return isValid(coup, 0, coup.size());
}

vector<int> findValidDiscountCoupons(vector<string> discounts)
{
    std::vector<int> validity;
    validity.reserve(discounts.size());
    for (const auto coup : discounts)
        validity.push_back(isValid(coup) ? 1 : 0);
    return validity;
}

#include <gtest/gtest.h>

using testing::Test;

struct AmazonFixture : public Test
{
    AmazonFixture()
    {
    }

    ~AmazonFixture()
    {
    }
};

TEST_F(AmazonFixture, AA)
{
    EXPECT_TRUE(isValid("AA"));
}

TEST_F(AmazonFixture, ABA)
{
    EXPECT_FALSE(isValid("ABA"));
}

TEST_F(AmazonFixture, ABBA)
{
    EXPECT_TRUE(isValid("ABBA"));
}

TEST_F(AmazonFixture, ACDA)
{
    EXPECT_FALSE(isValid("ACDA"));
}

TEST_F(AmazonFixture, CDABCCBBBADC)
{
    EXPECT_TRUE(isValid("CDABCCBBBADC"));
}

TEST_F(AmazonFixture, CDABCCBBDDBADCABCCBACDBAABDC)
{
    EXPECT_TRUE(isValid("CDABCCBBDDBADCABCCBACDBAABDC"));
}

TEST_F(AmazonFixture, CDABCCBBDDBADCABCCBACDBAABDCABCAAAABBCBA)
{
    EXPECT_TRUE(isValid("CDABCCBBDDBADCABCCBACDBAABDCABCAAAABBCBA"));
}

TEST_F(AmazonFixture, AFGSCDABCCBBDDBADCABCCBACDBAABDCABCAAAABBCBASGFA)
{
    EXPECT_TRUE(isValid("AFGSCDABCCBBDDBADCABCCBACDBAABDCABCAAAABBCBASGFA"));
}

TEST_F(AmazonFixture, AAABAABBBAAA)
{
    EXPECT_TRUE(isValid("AAABAABBBAAA"));
}

TEST_F(AmazonFixture, AABBAACCAA)
{
    EXPECT_TRUE(isValid("AABBAACCAA"));
}
