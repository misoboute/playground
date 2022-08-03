//
// Binary trees are already defined with this interface:
// template<typename T>
// struct Tree {
//   Tree(const T &v) : value(v), left(nullptr), right(nullptr) {}
//   T value;
//   Tree *left;
//   Tree *right;
// };

// Write a function to compare that the leaves ("fringe") of two binary trees to determine 
// whether they are the same list of leaves when visited left-to-right. The structure or 
// balance of the trees does not matter; only the number, order, and value of the leaves is important.
// Fringe: a list of Nodes without children
// Test case:
//              A                   B
//             V=12               V=32
//         V=21   V=11       V=88      V=99
//      V=77 V=33              V=77  V=33 V=11  
//
// Fringe for A = 77,33,11, B = 77,33,11 : returns true

// Test 1:
// a:
// {
//     "value": 12,
//     "left": {
//         "value": 21,
//         "left": {
//             "value": 77,
//             "left": null,
//             "right": null
//         },
//         "right": {
//             "value": 33,
//             "left": null,
//             "right": null
//         }
//     },
//     "right": {
//         "value": 11,
//         "left": null,
//         "right": null
//     }
// }
// b:
// {
//     "value": 32,
//     "left": {
//         "value": 88,
//         "left": null,
//         "right": {
//             "value": 77,
//             "left": null,
//             "right": null
//         }
//     },
//     "right": {
//         "value": 99,
//         "left": {
//             "value": 33,
//             "left": null,
//             "right": null
//         },
//         "right": {
//             "value": 11,
//             "left": null,
//             "right": null
//         }
//     }
// }
// Expected Output:
// true

// Test 2: 
// Input:
// a:
// {
//     "value": 5,
//     "left": {
//         "value": 3,
//         "left": {
//             "value": 4,
//             "left": null,
//             "right": null
//         },
//         "right": {
//             "value": 6,
//             "left": {
//                 "value": 7,
//                 "left": null,
//                 "right": null
//             },
//             "right": null
//         }
//     },
//     "right": {
//         "value": 9,
//         "left": {
//             "value": 8,
//             "left": null,
//             "right": {
//                 "value": 14,
//                 "left": null,
//                 "right": null
//             }
//         },
//         "right": {
//             "value": 1,
//             "left": null,
//             "right": null
//         }
//     }
// }
// b:
// {
//     "value": 8,
//     "left": {
//         "value": 13,
//         "left": {
//             "value": 4,
//             "left": null,
//             "right": null
//         },
//         "right": {
//             "value": 7,
//             "left": null,
//             "right": null
//         }
//     },
//     "right": {
//         "value": 9,
//         "left": {
//             "value": 14,
//             "left": null,
//             "right": null
//         },
//         "right": {
//             "value": 11,
//             "left": null,
//             "right": null
//         }
//     }
// }
// Expected Output:
// false

// Test 3:
// Input:
// a:
// {
//     "value": 1,
//     "left": null,
//     "right": null
// }
// b:
// {
//     "value": 1,
//     "left": null,
//     "right": null
// }
// Expected Output:
// true

#include <stack>
#include <vector>

template<typename T>
struct Tree {
  Tree(const T &v) : value(v), left(nullptr), right(nullptr) {}
  T value;
  Tree *left;
  Tree *right;
};

template <typename T>
class TreeLeafIterator
{
public:
    using ValueT = T;
    using TreeT = Tree<ValueT>;

    TreeLeafIterator(TreeT const * t)
    {
        m_Stack.push(t);
        ++(*this);
    }

    TreeLeafIterator<ValueT>& operator++()
    {
        while (!m_Stack.empty())
        {
            auto a = m_Stack.top();
            m_Stack.pop();
            if (a->left == nullptr && a->right == nullptr)
            {
                m_Tree = a;
                return *this;
            }
            if (a->left)
            {
                m_Stack.push(a->left);
            }
            if (a->right)
            {
                m_Stack.push(a->right);
            }
        }
        m_Tree = nullptr;
        return *this;
    }

    ValueT const & operator*() const
    {
        return m_Tree->value;
    }

    bool operator==(const TreeLeafIterator<ValueT>& other)
    {
        return m_Stack == other.m_Stack && m_Tree == other.m_Tree;
    }

    bool operator!=(const TreeLeafIterator<ValueT>& other)
    {
        return m_Stack != other.m_Stack || m_Tree != other.m_Tree;
    }

    static TreeLeafIterator<ValueT> End()
    {
        return TreeLeafIterator<ValueT>();
    }

private:
    TreeLeafIterator()
    {
    }

    TreeT const * m_Tree = nullptr;
    std::stack< TreeT const *, std::vector<TreeT const *> > m_Stack;
};

void fringes(Tree<int>* t, std::vector<int>& f)
{
    std::vector< Tree<int>* > stack;
    stack.push_back(t);
    while (!stack.empty())
    {
        auto a = stack.back();
        stack.pop_back();
        if (a->left == nullptr && a->right == nullptr)
        {
            f.push_back(a->value);
        }
        if (a->left)
        {
            stack.push_back(a->left);
        }
        if (a->right)
        {
            stack.push_back(a->right);
        }
    }
}

// The solution I came up with during the interview
// bool solution(Tree<int> * a, Tree<int> * b)
// {
//     std::vector<int> fa, fb;
//     fringes(a, fa);
//     fringes(b, fb);
//     return fa == fb;
// }

bool solution(Tree<int> * a, Tree<int> * b)
{
    using It = TreeLeafIterator<int>;
    auto ia = It(a), ib = It(b), end = It::End();
    for (; ia != end && ib != end; ++ia, ++ib)
    {
        if (*ia != *ib)
        {
            return false;
        }
    }
    return ia == end && ib == end;
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::Test;
using testing::ElementsAre;

struct CompareTreeLeavesFixture : public Test
{
    CompareTreeLeavesFixture()
    {
    }

    ~CompareTreeLeavesFixture()
    {
    }

    template<typename T>
    struct TreeContainer
    {
        using ValueT = T;
        using TreeT = Tree<ValueT>;

        ~TreeContainer()
        {
            if (root == nullptr)
                return;

            auto nodesToDelete = std::vector<TreeT*>{root};
            while (!nodesToDelete.empty())
            {
                auto cur = nodesToDelete.back();
                nodesToDelete.pop_back();
                if (cur->left != nullptr)
                    nodesToDelete.push_back(cur->left);
                if (cur->right != nullptr)
                    nodesToDelete.push_back(cur->right);
                delete cur;
            }
        }

        TreeT* root = nullptr;
    };
};

TEST_F(CompareTreeLeavesFixture, T1)
{
    TreeContainer<int> a;
    a.root = new Tree<int>(12);
    a.root->left = new Tree<int>(21);
    a.root->left->left = new Tree<int>(77);
    a.root->left->right = new Tree<int>(33);
    a.root->right = new Tree<int>(11);

    TreeContainer<int> b;
    b.root = new Tree<int>(32);
    b.root->left = new Tree<int>(88);
    b.root->left->right = new Tree<int>(77);
    b.root->right = new Tree<int>(99);
    b.root->right->left = new Tree<int>(33);
    b.root->right->right = new Tree<int>(11);

    EXPECT_TRUE(solution(a.root, b.root));
}

TEST_F(CompareTreeLeavesFixture, T2)
{
    TreeContainer<int> a;
    a.root = new Tree<int>(5);
    a.root->left = new Tree<int>(3);
    a.root->left->left = new Tree<int>(4);
    a.root->left->right = new Tree<int>(6);
    a.root->left->right->left = new Tree<int>(7);
    a.root->right = new Tree<int>(9);
    a.root->right->left = new Tree<int>(8);
    a.root->right->left->right = new Tree<int>(14);
    a.root->right->right = new Tree<int>(1);

    TreeContainer<int> b;
    b.root = new Tree<int>(8);
    b.root->left = new Tree<int>(13);
    b.root->left->left = new Tree<int>(4);
    b.root->left->right = new Tree<int>(7);
    b.root->right = new Tree<int>(9);
    b.root->right->left = new Tree<int>(14);
    b.root->right->right = new Tree<int>(11);

    EXPECT_FALSE(solution(a.root, b.root));
}

TEST_F(CompareTreeLeavesFixture, T3)
{
    TreeContainer<int> a;
    a.root = new Tree<int>(1);

    TreeContainer<int> b;
    b.root = new Tree<int>(1);

    EXPECT_TRUE(solution(a.root, b.root));
}
