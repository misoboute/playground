#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class LetterBinaryTree
{
public:
    class Node;

    using NodeSPtr = std::shared_ptr<Node>;
    using NodeWPtr = std::weak_ptr<Node>;

    enum class Error
    {
        InvalidInput,
        DuplicatePair,
        BeyondBinary,
        MultipleRoots,
        Cycles,
        Ok,
        NotInitialized,
    };

    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        Node(char value)
            : m_Value(value)
        {
        }

        char GetValue() const { return m_Value; }
        bool IsRoot() const { return m_Parent.lock() == nullptr; }
        NodeSPtr GetParent() { return m_Parent.lock(); }
        NodeSPtr GetLeft() { return m_Left; }
        NodeSPtr GetRight() { return m_Right; }

        Error AddChild(const NodeSPtr child)
        {
            if (m_Left == child || m_Right == child || m_UnwantedChildren.find(child) != m_UnwantedChildren.cend())
                return Error::DuplicatePair;
            if (m_Left && m_Right)
            {
                m_UnwantedChildren.insert(child);
                return Error::BeyondBinary;
            }
            if (!m_Left || m_Left->GetValue() > child->GetValue())
            {
                m_Right = m_Left;
                m_Left = child;
            }
            else
                m_Right = child;
            if (child->m_Parent.lock())
                return Error::Cycles;
            child->m_Parent = shared_from_this();
            return Error::Ok;
        }

    private:
        char m_Value;
        NodeSPtr m_Left;
        NodeSPtr m_Right;
        NodeWPtr m_Parent;
        std::unordered_set<NodeSPtr> m_UnwantedChildren;
    };

    LetterBinaryTree()
        : m_Nodes(m_MaxNodes, nullptr),
        m_NodesVisited(m_MaxNodes, false),
        m_Error(Error::NotInitialized)
    {
    }

    void ReadFromStream(std::istream& s)
    {
        bool spaceBeforePair = false;
        while (s.good())
        {
            if (spaceBeforePair)
            {
                auto sp = s.get();
                if (sp == std::istream::traits_type::eof())
                    break;
                if (static_cast<char>(sp) != ' ')
                {
                    SetError(Error::InvalidInput);
                    return;
                }
            }
            auto ok = false;
            auto p = ReadPairFromStream(s, ok);
            if (!ok)
            {
                SetError(Error::InvalidInput);
                return;
            }
            auto error = AddPairToTree(p.first, p.second);
            SetError(error);
            spaceBeforePair = true;
        }
        FindRootNode();
        DetectCycles();
        SetError(Error::Ok);
    }

    void WriteSExprToStream(std::ostream& s)
    {
        if (m_Error != Error::Ok)
        {
            s << GetErrorStr();
            return;
        }
        auto str = GetNodeSExprCatchCycles(m_Root);
        if (m_Error != Error::Ok)
            s << GetErrorStr();
        else
            s << str;
    }

    NodeSPtr GetRoot()
    {
        return m_Root;
    }

    Error GetError()
    {
        return m_Error;
    }

    std::string GetErrorStr()
    {
        switch (m_Error)
        {
        case Error::NotInitialized:
            return "Not Initialized";
        case Error::Ok:
            return "OK";
        case Error::InvalidInput:
            return "E1";
        case Error::DuplicatePair:
            return "E2";
        case Error::BeyondBinary:
            return "E3";
        case Error::MultipleRoots:
            return "E4";
        case Error::Cycles:
            return "E5";
        }
        return "";
    }

private:
    static constexpr const size_t m_MaxNodes = 'Z' - 'A' + 1;

    static char IndexToValue(size_t index) { return 'A' + index; }
    static size_t ValueToIndex(char value) { return value - 'A'; }

    void SetError(Error err)
    {
        if (m_Error > err)
            m_Error = err;
    }

    std::pair<char, char> ReadPairFromStream(std::istream& s, bool& ok)
    {
        ok = false;
        auto p = std::make_pair('\0', '\0');
        auto c = '\0';
        c = static_cast<char>(s.get());
        if (c != '(')
            return p;
        c = static_cast<char>(s.get());
        if (c > 'Z' || c < 'A')
            return p;
        p.first = c;
        c = static_cast<char>(s.get());
        if (c != ',')
            return p;
        c = static_cast<char>(s.get());
        if (c > 'Z' || c < 'A')
            return p;
        p.second = c;
        c = static_cast<char>(s.get());
        if (c != ')')
            return p;
        ok = true;
        return p;
    }

    Error AddPairToTree(char parent, char child)
    {
        auto parentIndex = ValueToIndex(parent);
        auto childIndex = ValueToIndex(child);
        if (m_Nodes[parentIndex] == nullptr)
            m_Nodes[parentIndex] = std::make_shared<Node>(parent);
        if (m_Nodes[childIndex] == nullptr)
            m_Nodes[childIndex] = std::make_shared<Node>(child);
        auto childNode = m_Nodes[childIndex];
        auto parentNode = m_Nodes[parentIndex];
        return parentNode->AddChild(childNode);
    }

    void FindRootNode()
    {
        for (const auto& n : m_Nodes)
        {
            if (!n)
                continue;
            if (n->IsRoot())
            {
                if (m_Root)
                    SetError(Error::MultipleRoots);
                else
                    m_Root = n;
            }
        }
        if (!m_Root)
            SetError(Error::Cycles);
    }

    void DetectCycles()
    {
        auto nodesVisited = std::vector<bool>(m_MaxNodes, false);
        for (auto i = 0; i < m_MaxNodes; ++i)
        {
            if (nodesVisited[i] || !m_Nodes[i])
                continue;
            auto nodesToVisit = std::vector<NodeSPtr>{ m_Nodes[i] };
            while (!nodesToVisit.empty())
            {
                auto currentNode = nodesToVisit.back();
                auto currentIndex = ValueToIndex(currentNode->GetValue());
                if (nodesVisited[currentIndex])
                {
                    SetError(Error::Cycles);
                    return;
                }
                nodesVisited[currentIndex] = true;
                nodesToVisit.pop_back();
                if (currentNode->GetLeft())
                    nodesToVisit.push_back(currentNode->GetLeft());
                if (currentNode->GetRight())
                    nodesToVisit.push_back(currentNode->GetRight());
            }
        }
    }

    std::string GetNodeSExprCatchCycles(const NodeSPtr& node)
    {
        if (!node || m_Error != Error::Ok)
            return "";
        std::ostringstream out;
        auto nodeIndex = ValueToIndex(node->GetValue());
        if (m_NodesVisited[nodeIndex])
        {
            SetError(Error::Cycles);
            return "";
        }
        m_NodesVisited[nodeIndex] = true;
        out << "(" << node->GetValue() << GetNodeSExprCatchCycles(node->GetLeft()) << 
            GetNodeSExprCatchCycles(node->GetRight()) << ")";
        return out.str();
    }

    std::vector<NodeSPtr> m_Nodes;
    std::vector<bool> m_NodesVisited;
    NodeSPtr m_Root;
    Error m_Error;
};

#if 0

int main() {
    LetterBinaryTree tree;
    tree.ReadFromStream(std::cin);
    tree.WriteSExprToStream(std::cout);
    return 0;
}

#endif

#include <gtest/gtest.h>

using testing::Test;

struct OptiverFixture : public Test
{
    OptiverFixture()
    {
    }

    ~OptiverFixture()
    {
    }

    std::string GetSExprFromInput(const std::string& inputStr)
    {
        std::istringstream input(inputStr);
        std::ostringstream output;
        m_Tree.ReadFromStream(input);
        m_Tree.WriteSExprToStream(output);
        return output.str();
    }

    LetterBinaryTree m_Tree;
};

TEST_F(OptiverFixture, E1_LeftUntrimmed)
{
    EXPECT_EQ(GetSExprFromInput(" (A,B) (B,C) (B,D) (D,E)"), "E1");
}

TEST_F(OptiverFixture, E1_RightUntrimmed)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D) (D,E) "), "E1");
}

TEST_F(OptiverFixture, E1_SpaceInTheMiddle)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B, D) (D,E)"), "E1");
}

TEST_F(OptiverFixture, E1_LowerCaseLetter)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,d) (D,E)"), "E1");
}

TEST_F(OptiverFixture, E1_MissingParen)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D (D,E)"), "E1");
}

TEST_F(OptiverFixture, E1_MissingSpace)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C)(B,D) (D,E)"), "E1");
}

TEST_F(OptiverFixture, E1_ExtraSpace)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D)  (D,E)"), "E1");
}

TEST_F(OptiverFixture, E1_ExtraSpaceAndDup)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (A,B) (B,D)  (D,E)"), "E1");
}

TEST_F(OptiverFixture, E2_Dup)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D) (D,E) (B,C)"), "E2");
}

TEST_F(OptiverFixture, E3_ThreeChildren)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D) (D,E) (B,X)"), "E3");
}

TEST_F(OptiverFixture, E2_ThreeChildrenAndADup)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D) (D,E) (B,X) (B,D)"), "E2");
}

TEST_F(OptiverFixture, E2_ThirdChildIsDup)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D) (D,E) (B,X) (E,H) (B,X)"), "E2");
}

TEST_F(OptiverFixture, E5_CycleToRoot)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D) (D,E) (E,H) (E,A)"), "E5");
}

TEST_F(OptiverFixture, E5_CycleToNonRootAncestor)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D) (D,E) (E,H) (E,B)"), "E5");
}

TEST_F(OptiverFixture, E5_CycleToNonRootNonAncestor)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,C) (B,D) (D,E) (E,H) (E,C)"), "E5");
}

TEST_F(OptiverFixture, E5_CycleOutOfOrder)
{
    EXPECT_EQ(GetSExprFromInput("(D,E) (B,D) (B,C) (E,H) (A,B) (E,A)"), "E5");
}

TEST_F(OptiverFixture, E3_CycleFormedByThirdChild)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (E,H) (E,M) (E,B) (B,C) (B,D) (D,E)"), "E3");
}

TEST_F(OptiverFixture, E4_MultipleRoots)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (E,H) (E,M) (B,C) (G,X) (B,D) (D,E)"), "E4");
}

TEST_F(OptiverFixture, E4_MultipleRootsWithUnwantedChild)
{
    EXPECT_EQ(GetSExprFromInput("(A,H) (H,I) (H,J) (A,C) (C,F) (C,K) (C,G) (B,D) (B,E)"), "E3");
}

TEST_F(OptiverFixture, E4_MultipleRootsWithCycle)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (E,H) (E,M) (B,C) (G,X) (B,D) (D,E) (G,Y)"), "E4");
}

TEST_F(OptiverFixture, NoError_Ordinary)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (A,C) (C,D) (D,E) (D,F) (C,G) (F,H)"), "(A(B)(C(D(E)(F(H)))(G)))");
}

TEST_F(OptiverFixture, NoError_Shuffled)
{
    EXPECT_EQ(GetSExprFromInput("(C,D) (D,E) (C,G) (A,C) (F,H) (D,F) (A,B)"), "(A(B)(C(D(E)(F(H)))(G)))");
}

TEST_F(OptiverFixture, NoError_Sample)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (B,D) (D,E) (A,C) (C,F) (E,G)"), "(A(B(D(E(G))))(C(F)))");
}

TEST_F(OptiverFixture, E5_Sample)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (A,C) (B,D) (D,C)"), "E5");
}

TEST_F(OptiverFixture, NoError_Sample0)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (A,C) (B,D) (D,E) (C,F) (E,G)"), "(A(B(D(E(G))))(C(F)))");
}

TEST_F(OptiverFixture, E3_Sample1)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (A,C) (A,D) (B,E)"), "E3");
}

TEST_F(OptiverFixture, E1_Sample2)
{
    EXPECT_EQ(GetSExprFromInput("awecihjaw;elkifjhasz"), "E1");
}

TEST_F(OptiverFixture, E4_Sample3)
{
    EXPECT_EQ(GetSExprFromInput("(A,B) (A,C) (X,Y) (X,Z)"), "E4");
}

TEST_F(OptiverFixture, RevealedToBeFailingDuringFinalInterview)
{
    EXPECT_EQ(GetSExprFromInput("(D,C) (D,B) (A,F) (F,E) (E,A)"), "E5");
}
