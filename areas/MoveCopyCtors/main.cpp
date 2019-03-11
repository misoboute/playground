#include <iostream>
#include <string>
#include <vector>

class Obj
{
public: 
    Obj(int m)
    : m_(m)
    {
        std::cout << "Obj default " << m_ << std::endl;
    }
    
    Obj(const Obj& other)
    : m_(other.m_)
    {
        std::cout << "Obj copy " << m_ << std::endl;
    }
    
    Obj(Obj&& other)
    : m_(other.m_)
    {
        other.m_ = 0;
        std::cout << "Obj move " << m_ << std::endl;
    }
    
    ~Obj()
    {
        std::cout << "Obj dtor " << m_ << std::endl;
    }
    
    int m_;
};

class A
{
public:
    A(Obj o)
    : o_(std::move(o)) {}

private:
    Obj o_;
};

class B
{
public:
    B(const Obj& o)
    : o_(o) {}

private:
    Obj o_;
};

class C
{
public:
    C(Obj&& o)
    : o_(std::move(o)) {}

private:
    Obj o_;
};

int main()
{
    Obj oA(1), oB(2), oC(3), oA4(4), oB5(5);
    std::cout << "Creating A" << std::endl;
    A a(oA);
    std::cout << "Creating A 4" << std::endl;
    A b(std::move(oA4));
    std::cout << std::endl << "Creating B" << std::endl;
    B c(oB);
    std::cout << std::endl << "Creating B 5" << std::endl;
    B d(std::move(oB5));
    std::cout << std::endl << "Creating C" << std::endl;
    C e(std::move(oC));
    std::cout << std::endl << "Done" << std::endl;
}
