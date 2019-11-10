#pragma once

#include <algorithm>
#include <random>
#include <vector>

namespace smop
{

    class RandomGenerator
    {
    public:
        RandomGenerator()
            : eng_(std::random_device()())
        {
        }

        std::vector<char> Bytes(size_t length)
        {
            auto bytes = std::vector<char>(length);
            auto dist = std::uniform_int_distribution<int>(-127, 127);
            std::generate(bytes.begin(), bytes.end(),
                [&dist, this] { return static_cast<char>(dist(eng_)); });
            return bytes;
        }

        std::vector<uint8_t> UBytes(size_t length)
        {
            auto ubytes = std::vector<uint8_t>(length);
            auto dist = std::uniform_int_distribution<int>(0, 255);
            std::generate(ubytes.begin(), ubytes.end(),
                [&dist, this] { return static_cast<char>(dist(eng_)); });
            return ubytes;
        }

        template<class IntT>
        IntT Integer(IntT low, IntT high)
        {
            return std::uniform_int_distribution<IntT>(low, high)(eng_);
        }

        template<class T>
        T* Pointer()
        {
            auto intPtr = Integer<uintptr_t>(
                std::numeric_limits<uintptr_t>::min(),
                std::numeric_limits<uintptr_t>::max());
            return reinterpret_cast<T*>(intPtr);
        }

        template<class RlT>
        RlT Real(RlT low, RlT high)
        {
            return std::uniform_real_distribution<RlT>(low, high)(eng_);
        }

        bool Bool()
        {
            return std::uniform_int_distribution<int>(0, 1)(eng_) == 1;
        }

        // Random string of alphabetical, numeric, and punctuation characters
        // for which std::isgraph(ch) is true.
        std::string GraphCharStr(size_t minLength, size_t maxLength,
            std::string const& excludeChars = "")
        {
            const auto len = Integer(minLength, maxLength);
            auto str = std::string(len, ' ');
            auto dist = std::uniform_int_distribution<int>(33, 126);
            for (int i = 0; i < len;)
            {
                auto ch = static_cast<char>(dist(eng_));
                if (excludeChars.find(ch) == excludeChars.npos)
                {
                    str[i++] = ch;
                }
            }
            return str;
        }

        template<typename ListT>
        void ShuffleList(ListT& list)
        {
            std::shuffle(list.begin(), list.end(), eng_);
        }

        template<typename ListT, typename GenFunc>
        auto PopulateList(
            size_t minSize, size_t maxSize, ListT& list, GenFunc genFunc)
        {
            const auto size = Integer(minSize, maxSize);
            list.reserve(size);
            std::generate_n(std::back_inserter(list), size, genFunc);
        }

    private:
        std::default_random_engine eng_;
    };

}
