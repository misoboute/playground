#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// Write a function that detects whether a string is _correct_.
// An empty string is not correct.
// If string `S` is correct, then the strings `(S)` and `[S]` are also correct.
// If strings `S` and `T` are correct, then the strings `ST` and `TS` are also 
// correct.
class Answer
{
public:
    //! checks that the given sequence is​​​​​​‌​​‌​​​​‌‌​‌​​‌‌​‌‌​‌‌‌​​ correct
    static bool check(string sequence)
    {
        if (sequence.empty())
        {
            return false;
        }
        resetCache(sequence.size());
        return isCorrect(sequence, 0, sequence.size());
    }

private:

    struct pair_hash
    {
        template <class T1, class T2>
        size_t operator() (const pair<T1, T2> &pair) const
        {
            return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
        }
    };

    using CacheT = unordered_map< pair<size_t, size_t>, bool, pair_hash>;

    static bool isCorrect(const string& s, int l, int u)
    {
        auto fromCache = cache_.find({l, u});
        if (fromCache != cache_.cend())
            return fromCache->second;
        if ((u - l) % 2 != 0)
            return cache_[{l, u}] = false;
        while (u > l && ((s[l] == '(' && s[u - 1] == ')') || (s[l] == '[' && s[u - 1] == ']')))
        {
            if (trySplitting(s, l, u))
                return cache_[{l, u}] = true;
            ++l, --u;
        }
        if (l >= u)
            return cache_[{l, u}] = true;
        return cache_[{l, u}] = trySplitting(s, l, u);
    }

    static void resetCache(size_t strSize)
    {
        cache_ = CacheT(strSize * strSize / 1024);
    }

    static bool trySplitting(string s, int l, int u)
    {
        if (u - l < 4)
            return false;
        for (size_t i = l + 2; i <= u - 2; i += 2)
        {
            if (isCorrect(s, l, i) && isCorrect(s, i, u))
                return true;
        }
        return false;
    }

    static CacheT cache_;
};

Answer::CacheT Answer::cache_;

#include <gtest/gtest.h>

using testing::Test;

struct GameloftFixture : public Test
{
    GameloftFixture()
    {
    }

    ~GameloftFixture()
    {
    }
};

TEST_F(GameloftFixture, Empty)
{
    EXPECT_FALSE(Answer::check(""));
}

TEST_F(GameloftFixture, Pp)
{
    EXPECT_TRUE(Answer::check("()"));
}

TEST_F(GameloftFixture, Bb)
{
    EXPECT_TRUE(Answer::check("[]"));
}

TEST_F(GameloftFixture, PBBp)
{
    EXPECT_FALSE(Answer::check("([[)"));
}

TEST_F(GameloftFixture, PPpBbp)
{
    EXPECT_TRUE(Answer::check("(()[])"));
}

TEST_F(GameloftFixture, PPpBPpBbbPpp)
{
    EXPECT_TRUE(Answer::check("(()[()[]]())"));
}

TEST_F(GameloftFixture, PPpbBPpBbbPpp)
{
    EXPECT_FALSE(Answer::check("(()][()[]]())"));
}
