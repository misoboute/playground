#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "transaction.hpp"

using namespace std;
using testing::Test;
using testing::ElementsAre;

class manager1;
class manager2;
std::ostream& operator<<(std::ostream& o, const manager1& m);
std::ostream& operator<<(std::ostream& o, const manager2& m);

class manager1
{
public:
    manager1()
    {
        map1_[2] = "original";
        map2_[0] = 2.78;
    }

    std::shared_ptr<transaction> load()
    {
        auto xact = std::make_shared<transaction_multistep>();
        
        xact->emplace_step< transaction_change_data< std::map<long, std::string> > >(map1_);
        std::cout << "After transaction man1::map1_...\n" << *this << std::endl;
        map1_.clear();
        map1_[3] = "changed";

        xact->emplace_step< transaction_change_data< std::map<long, double> > >(map2_);
        map2_.clear();
        map2_[1] = 3.14;
        
        return xact;
    }

    std::ostream& print(std::ostream& o) const
    {
        auto printItem = [&o](const auto& i)
        {
            o << "    [" << i.first << "] = " << i.second << "\n";
        };
        o << "manager1::map1_ :\n";
        std::for_each(map1_.cbegin(), map1_.cend(), printItem);
        o << "manager1::map2_ :\n";
        std::for_each(map2_.cbegin(), map2_.cend(), printItem);
        return o;
    }

private:
    std::map<long, std::string> map1_;
    std::map<long, double> map2_;
};

class manager2
{
public:
    manager2()
    {
        map1_[2] = "original";
        map2_[0] = 2.78;
    }

    std::shared_ptr<transaction> load()
    {
        auto xact = std::make_shared<transaction_multistep>();

        xact->emplace_step< transaction_change_data< std::map<long, std::string> > >(map1_);
        map1_.clear();
        map1_[3] = "changed";

        xact->emplace_step< transaction_change_data< std::map<long, double> > >(map2_);
        map2_.clear();
        map2_[1] = 3.14;

        return xact;
    }

    std::ostream& print(std::ostream& o) const
    {
        auto printItem = [&o](const auto& i)
        {
            o << "    [" << i.first << "] = " << i.second << "\n";
        };
        o << "manager1::map1_ :\n";
        std::for_each(map1_.cbegin(), map1_.cend(), printItem);
        o << "manager1::map2_ :\n";
        std::for_each(map2_.cbegin(), map2_.cend(), printItem);
        return o;
    }

private:
    std::map<long, std::string> map1_;
    std::map<long, double> map2_;
};

std::ostream& operator<<(std::ostream& o, const manager1& m)
{
    return m.print(o);
}

std::ostream& operator<<(std::ostream& o, const manager2& m)
{
    return m.print(o);
}

struct TransactionFixture : public Test
{
    TransactionFixture()
    {
    }

    ~TransactionFixture()
    {
    }
};

TEST_F(TransactionFixture, T1)
{
    manager1 man1;
    manager2 man2;
    std::cout << "Before transaction ...\n" << man1 << man2 << std::endl;
    try
    {
        transaction_multistep xact{};
        xact.add_step(man1.load());
        std::cout << "Before exception ...\n" << man1 << man2 << std::endl;
        throw "something";
        xact.add_step(man2.load());
        xact.finalize();
        
        transaction_multistep xact2
        {
            man1.load(),
            man2.load()
        };
        xact2.finalize();
    }
    catch(...)
    {
        std::cerr << "Caugth exception!" << std::endl;
    }
    std::cout << "After transaction ...\n" << man1 << man2 << std::endl;

    EXPECT_TRUE(true);
}
