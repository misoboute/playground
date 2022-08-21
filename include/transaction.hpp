#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class transaction
{
public:
    virtual ~transaction() = default;
    transaction(const transaction&) = delete;
    transaction(transaction&&) = delete;
    transaction& operator=(const transaction&) = delete;
    transaction& operator=(transaction&&) = delete;

    virtual void finalize() = 0;

protected:
    transaction() = default;
};

template <class T>
class transaction_change_data : public transaction
{
public:
    transaction_change_data(T& data)
        : data_ptr_(&data)
    {
        std::swap(original_data_, data);
    }

    ~transaction_change_data()
    {
        if (pending_)
        {
            *data_ptr_ = original_data_;
        }
    }

    void finalize() override
    {
        pending_ = false;
        original_data_ = T{};
    }

private:
    T* data_ptr_;
    T original_data_;
    bool pending_ = true;
};

class transaction_multistep : public transaction
{
public:
    using step_ptr_t = std::shared_ptr<transaction>;
    
    transaction_multistep() { }
    
    transaction_multistep(std::initializer_list<step_ptr_t> steps)
        : steps_(steps.begin(), steps.end())
    {
    }
    
    void add_step(step_ptr_t step)
    {
        steps_.push_back(step);
    }
    
    template <class U, class... Args>
    void emplace_step(Args&& ... args)
    {
        steps_.emplace_back(std::make_shared<U>(std::forward<Args>(args)...));
    }

    void finalize() override
    {
        for (auto& s : steps_)
        {
            s->finalize();
        }
    }

private:
    std::vector<step_ptr_t> steps_;
};
