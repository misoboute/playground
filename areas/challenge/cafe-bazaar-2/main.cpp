#include <condition_variable>
#include <mutex>
#include <vector>

#include <gtest/gtest.h>

using testing::Test;

template <typename T, size_t S>
class Queue
{
public:
    void enq(T elem)
    {
        std::unique_lock<std::mutex> spaceAvLock(mut_);
        while (end_ - beg_ >= S)
        {
            spaceAvailableVar_.wait(spaceAvLock);
        }

        buffer_[end_ % S] = elem;
        ++end_;
        dataAvailableVar_.notify_one();
    }

    T deq()
    {
        std::unique_lock<std::mutex> dataAvLock(mut_);
        while (end_ == beg_)
        {
            dataAvailableVar_.wait(dataAvLock);
        }

        auto elem = buffer_[beg_ % S];
        ++beg_;
        spaceAvailableVar_.notify_one();
        return elem;
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mut_);
        return end_ - beg_;
    }

private:
    std::vector<T> buffer_ { S, T() };
    size_t beg_ = 0, end_ = 0;
    mutable std::mutex mut_;
    std::condition_variable dataAvailableVar_;
    std::condition_variable spaceAvailableVar_;
};

struct CafeBazaarFixture : public Test
{
    CafeBazaarFixture()
    {
    }

    ~CafeBazaarFixture()
    {
    }
};

TEST_F(CafeBazaarFixture, T1)
{
    Queue<int, 5> q;
    EXPECT_EQ(q.size(), 0);
    q.enq(1);
    EXPECT_EQ(q.size(), 1);
    q.enq(2);
    EXPECT_EQ(q.size(), 2);
    q.enq(3);
    EXPECT_EQ(q.size(), 3);
    q.enq(4);
    EXPECT_EQ(q.size(), 4);
    q.enq(5);
    EXPECT_EQ(q.size(), 5);
    EXPECT_EQ(q.deq(), 1);
    EXPECT_EQ(q.size(), 4);
    EXPECT_EQ(q.deq(), 2);
    EXPECT_EQ(q.size(), 3);
    EXPECT_EQ(q.deq(), 3);
    EXPECT_EQ(q.size(), 2);
    EXPECT_EQ(q.deq(), 4);
    EXPECT_EQ(q.size(), 1);
    EXPECT_EQ(q.deq(), 5);
    EXPECT_EQ(q.size(), 0);
    q.enq(6);
    EXPECT_EQ(q.size(), 1);
    q.enq(7);
    EXPECT_EQ(q.size(), 2);
    q.enq(8);
    EXPECT_EQ(q.size(), 3);
    EXPECT_EQ(q.deq(), 6);
    EXPECT_EQ(q.size(), 2);
    EXPECT_EQ(q.deq(), 7);
    EXPECT_EQ(q.size(), 1);
    EXPECT_EQ(q.deq(), 8);
    EXPECT_EQ(q.size(), 0);

    std::cout << "tests done" << std::endl;
}
