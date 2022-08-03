#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

class Card
{
    public:
        Card() {}
        
        explicit Card(char const* cardName)
        {
            SetCardName(cardName);
        }
        
        void SetCardName(char const* cardName)
        {
            if (cardName != nullptr)
            {
                mCardName = cardName;
            }
        }

        char const* GetCardName() const
        {
            return mCardName.c_str();
        }

    private:
        std::string mCardName;
};

class CardDeck
{
public:
    template<typename Str>
    explicit CardDeck(std::initializer_list<Str> names)
    {
        mCards.reserve(names.size());
        for (const auto& name : names)
        {
            mCards.emplace_back(name);
        }
        mSortedCards = mCards;
    }
    
    void Sort();
    void Shuffle();
    const std::vector<Card>& GetCards() const;
    void PrintCardList() const;
    
private:
    std::vector<Card> mCards;
    std::vector<Card> mSortedCards;
};

// int main(int argc, char* argv[])
// {
//     auto myDeck = CardDeck{
//         "AD", "KD", "QD", "JD", "10D", "9D", "8D", "7D", "6D", "5D", "4D", "3D", "2D",
//         "AH", "KH", "QH", "JH", "10H", "9H", "8H", "7H", "6H", "5H", "4H", "3H", "2H", 
//         "AS", "KS", "QS", "JS", "10S", "9S", "8S", "7S", "6S", "5S", "4S", "3S", "2S", "JK",
//         "AC", "KC", "QC", "JC", "10C", "9C", "8C", "7C", "6C", "5C", "4C", "3C", "2C"
//     };
    
//     int numDecks = 0;
//     std::cin >> numDecks;
    
//     for (int i = 0; i < numDecks; i++)
//     {
//         myDeck.Shuffle();
//         myDeck.PrintCardList();
//     }
    
//     return 0;
// }

void CardDeck::Sort()
{
    mCards = mSortedCards;
}

void CardDeck::Shuffle()
{
    std::random_device randDev;
    const auto deckSize = mSortedCards.size();
    auto cardsCopy = mSortedCards;
    for (int i = deckSize - 1; i >= 0; --i)
    {
        auto randDist = std::uniform_int_distribution<int>(0, i);
        auto randIndex = randDist(randDev);
        mCards[i] = cardsCopy[randIndex];
        std::swap(cardsCopy[randIndex], cardsCopy[i]);
    }
}

const std::vector<Card>& CardDeck::GetCards() const
{
    return mCards;
}

void CardDeck::PrintCardList() const
{
    std::cout << "CardList" << std::endl;
    for (auto const& card : mCards)
    {
        std::cout << card.GetCardName() << std::endl;
    }
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::UnorderedElementsAreArray;
using testing::Test;

struct SkyBoxFixture : public Test
{
    SkyBoxFixture()
    {

    }

    ~SkyBoxFixture()
    {
    }

    CardDeck m_Deck {
        "AD", "KD", "QD", "JD", "10D", "9D", "8D", "7D", "6D", "5D", "4D", "3D", "2D",
        "AH", "KH", "QH", "JH", "10H", "9H", "8H", "7H", "6H", "5H", "4H", "3H", "2H", 
        "AS", "KS", "QS", "JS", "10S", "9S", "8S", "7S", "6S", "5S", "4S", "3S", "2S", "JK",
        "AC", "KC", "QC", "JC", "10C", "9C", "8C", "7C", "6C", "5C", "4C", "3C", "2C"
    };
};

bool operator==(const Card& c1, const Card& c2)
{
    return strcmp(c1.GetCardName(), c2.GetCardName()) == 0;
}

TEST_F(SkyBoxFixture, T1)
{
    auto d1 = m_Deck.GetCards();
    m_Deck.Shuffle();
    auto d2 = m_Deck.GetCards();
    std::unique(d2.begin(), d2.end());
    EXPECT_THAT(d2, UnorderedElementsAreArray(d1.cbegin(), d1.cend()));
}
