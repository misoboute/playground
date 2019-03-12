#include <iostream>
#include <utility>

namespace my 
{

template <typename T>
class shared_ptr
{
public:
    shared_ptr(T* ptr)
    : ctl_(new ctl_block_t(ptr))
    {
        std::cout << "Managing object " << *ctl_->ptr_ << ", ref = " 
            << ctl_->ref_ << std::endl;
    }
    
    shared_ptr(const my::shared_ptr<T>& other)
    : ctl_(other.ctl_)
    {
        ++ctl_->ref_;
        std::cout << "New ref to object " << *ctl_->ptr_ << ", ref = " 
            << ctl_->ref_ << std::endl;
    }
    
    shared_ptr(my::shared_ptr<T>&& other)
    : ctl_(other.ctl_)
    {
        std::cout << "Moving ref to object " << *ctl_->ptr_ << ", ref = " 
            << ctl_->ref_ << std::endl;
        other.ctl_ = nullptr;
    }
    
    ~shared_ptr()
    {
        if (ctl_ == nullptr)
        {
            std::cout << "Destroying an empty ref" << std::endl;
            return;
        }
        std::cout << "Destroying last ref to object " << *ctl_->ptr_ << 
            ", ref = " << ctl_->ref_ << std::endl;
        if (--ctl_->ref_ == 0)
        {
            std::cout << "Destroying object " << *ctl_->ptr_ << std::endl;
            delete ctl_;
        }
    }

private:    
    struct ctl_block
    {
        ctl_block(T* ptr)
        : ptr_(ptr)
        {
        }

        ~ctl_block()
        {
            if (ptr_ != nullptr)
            {
                delete ptr_;
            }
        }

        T * ptr_;
        size_t ref_ = 1;
    };

    using ctl_block_t = typename shared_ptr<T>::ctl_block;

    ctl_block_t * ctl_;
};

template <typename T>
class unique_ptr
{
public: 
    unique_ptr(T* ptr)
    : ptr_(ptr)
    {
        std::cout << "Managing object " << *ptr_ << std::endl;
    }
    
    unique_ptr(unique_ptr<T>&& other)
    : ptr_(other.ptr_)
    {
        std::cout << "Moving ref to object " << *ptr_ << std::endl;
        other.ptr_ = nullptr;
    }
    
    ~unique_ptr()
    {
        if (ptr_ == nullptr)
        {
            std::cout << "Destroying an empty unique_ptr" << std::endl;
            return;
        }
        std::cout << "Destroying object " << *ptr_ << std::endl;
        delete ptr_;
    }
    
private:
    T * ptr_;
};

}

using int_sptr = my::shared_ptr<int>;
using int_uptr = my::unique_ptr<int>;

class A
{
public:
    A(int_sptr o)
    : o_(std::move(o)) 
    {
        std::cout << "A: Pass shared_ptr by value then move" << std::endl;
    }

private:
    int_sptr o_;
};

class B
{
public:
    B(const int_sptr& o)
    : o_(o) 
    {
        std::cout << 
            "B: Pass shared_ptr by const ref and don't move" << std::endl;
    }

private:
    int_sptr o_;
};

class C
{
public:
    C(int_sptr&& o)
    : o_(std::move(o))
    {
        std::cout << "C: Pass shared_ptr by rvalue ref then move" << std::endl;
    }
    
private:
    int_sptr o_;
};

class D
{
public:
    D(const int_sptr& o)
    : o_(std::move(o))
    {
        std::cout << "D: Pass shared_ptr by const ref then move" << std::endl;
    }
    
private:
    int_sptr o_;
};

class E
{
public:
    E(int_uptr o)
    : o_(std::move(o))
    {
        std::cout << "E: Pass unique_ptr by value then move" << std::endl;
    }
    
private:
    int_uptr o_;
};

class F
{
public:
    F(int_uptr&& o)
    : o_(std::move(o))
    {
        std::cout << "F: Pass unique_ptr by rvalue ref then move" << std::endl;
    }
    
private:
    int_uptr o_;
};

int main()
{
    {
        std::cout << std::endl << "Creating A with lvalue 1" << std::endl;
        int_sptr s(new int(1));
        A a(s);
    }

    {
        std::cout << std::endl << "Creating A with rvalue 1" << std::endl;
        A a(int_sptr(new int(1)));
    }

    {
        std::cout << std::endl << "Creating B with lvalue 2" << std::endl;
        int_sptr s(new int(2));
        B b(s);
    }

    {
        std::cout << std::endl << "Creating B with rvalue 2" << std::endl;
        B b(int_sptr(new int(2)));
    }

    std::cout << std::endl << 
        "Creating C with lvalue is not possible" << std::endl;
    
    {
        std::cout << std::endl << "Creating C with rvalue 3" << std::endl;
        C c(int_sptr(new int(3)));
    }

    {
        std::cout << std::endl << "Creating D with lvalue 4" << std::endl;
        int_sptr s(new int(4));
        D d(s);
    }

    {
        std::cout << std::endl << "Creating D with rvalue 4" << std::endl;
        D d(int_sptr(new int(4)));
    }

    std::cout << std::endl << 
        "Creating E with lvalue is not possible" << std::endl;

    {
        std::cout << std::endl << "Creating E with rvalue 5" << std::endl;
        E e(int_uptr(new int(5)));
    }

    std::cout << std::endl << 
        "Creating F with lvalue is not possible" << std::endl;

    {
        std::cout << std::endl << "Creating F with rvalue 6" << std::endl;
        F f(int_uptr(new int(6)));
    }
}
