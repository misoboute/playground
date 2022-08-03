// Given the root node of a binary search tree of integers and an integer, 
// return the pointer to the node with the value of the given integer value or 
// nullptr if not found.

class Node
{
public:
    Node(int v)
    : left(nullptr),
    right(nullptr),
    value(v)
    {
    }

    Node* left;
    Node* right;
    int value;

    Node* find(int v);
};

Node* Node::find(int v)
{
    auto n = this;

    while (n != nullptr)
    {
        if (v > n->value)
            n = n->right;
        else if (v < n->value)
            n = n->left;
        else
            return n;
    }
    return nullptr;
}

#include <vector>

#include <gtest/gtest.h>

using testing::Test;

struct TreeBinarySearchFixture : public Test
{
    TreeBinarySearchFixture()
    {
    }

    ~TreeBinarySearchFixture()
    {
    }

    struct Tree
    {
        Tree& Insert(int v)
        {
            if (root == nullptr)
            {
                root = new Node(v);
                return *this;
            }
            
            auto n = root;
            while (true)
            {
                if (v > n->value)
                {
                    if (n->right == nullptr)
                    {
                        n->right = new Node(v);
                        break;
                    }
                    else
                        n = n->right;
                }
                else if (v < n->value)
                {
                    if (n->left == nullptr)
                    {
                        n->left = new Node(v);
                        break;
                    }
                    else
                        n = n->left;
                }
                else
                    break;
            }

            return *this;
        }

        ~Tree()
        {
            if (root == nullptr)
                return;

            auto nodesToDelete = std::vector<Node*>{root};
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

        Node* root = nullptr;
    };
};

TEST_F(TreeBinarySearchFixture, T1)
{
    auto nums = std::vector<int> { 9, 7, 13, 15, 17, 5, 6, 2, 1, 8, -4, 30
        , 30, 45, -12, 21, -8, -9, -11, 33, 24 };
    auto nonExistentNums = std::vector<int> { 3, 4, 10, 11, 12, 14, 16, 18, 23
        , 25, 29, 31, 32, 34, 44, 46, -15, -6 };
    Tree tree;
    for (auto v : nums)
        tree.Insert(v);
    for (auto v : nums)
    {
        EXPECT_EQ(tree.root->find(v)->value, v);
    }
    for (auto v : nonExistentNums)
    {
        EXPECT_EQ(tree.root->find(v), nullptr);
    }
}
