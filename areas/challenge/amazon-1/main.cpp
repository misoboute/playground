#include <vector>

using namespace std;

class SinglyLinkedListNode {
public:
    int data;
    SinglyLinkedListNode *next;

    SinglyLinkedListNode(int node_data) {
        this->data = node_data;
        this->next = nullptr;
    }
};

class SinglyLinkedList {
public:
    SinglyLinkedListNode *head;
    SinglyLinkedListNode *tail;

    SinglyLinkedList() {
        this->head = nullptr;
        this->tail = nullptr;
    }

    void insert_node(int node_data) {
        SinglyLinkedListNode* node = new SinglyLinkedListNode(node_data);

        if (!this->head) {
            this->head = node;
        } else {
            this->tail->next = node;
        }

        this->tail = node;
    }
};

/*
 * Complete the 'maximumPages' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_SINGLY_LINKED_LIST head as parameter.
 */

/*
 * For your reference:
 *
 * SinglyLinkedListNode {
 *     int data;
 *     SinglyLinkedListNode* next;
 * };
 *
 */

int maximumPages(SinglyLinkedListNode* head)
{
    auto nodeCount = 0;
    for (auto it = head; it != nullptr; it = it->next)
        ++nodeCount;
    const auto half = nodeCount / 2;
    SinglyLinkedListNode* prev = nullptr;
    auto cur = head;
    auto next = head->next;
    for (auto i = 0; i < half; ++i)
    {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    auto headingLeft = prev;
    auto headingRight = cur;
    auto maxPages = 0;
    while (headingLeft != nullptr && headingRight != nullptr)
    {
        const auto curPages = headingLeft->data + headingRight->data;
        if (maxPages < curPages)
            maxPages = curPages;
        headingRight = headingRight->next;
        headingLeft = headingLeft->next;
    }
    return maxPages;
}

#include <gtest/gtest.h>

#include "RandomGenerator.h"

using testing::Test;

struct OptiverFixture : public Test
{
    OptiverFixture()
        : m_RandIntGenFun([&r = m_Rand]() { return r.Integer<int>(1, 1000000000); })
    {
    }

    ~OptiverFixture()
    {
    }

    int maximumPagesVec(const std::vector<int>& vec)
    {
        int maxPages = 0;
        for (auto i = 0; i < vec.size() / 2; ++i)
        {
            auto pagesToday = vec[i] + vec[vec.size() - i - 1];
            if (pagesToday > maxPages)
            {
                maxPages = pagesToday;
            }
        }
        return maxPages;
    }

    SinglyLinkedList VectorToSinglyLinkedList(const std::vector<int>& vec)
    {
        SinglyLinkedList list;
        for (auto i : vec)
        {
            list.insert_node(i);
        }
        return list;
    }

    RandomGenerator m_Rand;
    std::function<int()> m_RandIntGenFun;
};

#define TEST_RAND_LIST(SIZE)    \
    auto numsVec = std::vector<int>();  \
    m_Rand.PopulateList(SIZE, SIZE, numsVec, m_RandIntGenFun);  \
    auto numsList = VectorToSinglyLinkedList(numsVec);  \
    EXPECT_EQ(maximumPages(numsList.head), maximumPagesVec(numsVec));


TEST_F(OptiverFixture, ListSize2)
{
    TEST_RAND_LIST(2)
}

TEST_F(OptiverFixture, ListSize4)
{
    TEST_RAND_LIST(4)
}

TEST_F(OptiverFixture, ListSize6)
{
    TEST_RAND_LIST(6)
}

TEST_F(OptiverFixture, ListSize20)
{
    TEST_RAND_LIST(20)
}

TEST_F(OptiverFixture, MaxListSize)
{
    TEST_RAND_LIST(100000)
}

TEST_F(OptiverFixture, MaxListSize1)
{
    TEST_RAND_LIST(100000)
}

TEST_F(OptiverFixture, MaxListSize2)
{
    TEST_RAND_LIST(100000)
}

TEST_F(OptiverFixture, MaxListSize3)
{
    TEST_RAND_LIST(100000)
}

TEST_F(OptiverFixture, MaxListSize4)
{
    TEST_RAND_LIST(100000)
}

TEST_F(OptiverFixture, MaxListSize5)
{
    TEST_RAND_LIST(100000)
}

TEST_F(OptiverFixture, HalfMaxListSize)
{
    TEST_RAND_LIST(50000)
}

