#pragma once

#include <iostream>
#include <type_traits>
#include <vector>

#include "TypeAliases.h"

inline std::ostream& operator<<(
    std::ostream& o, smop::DateTime::duration dur)
{
    auto unit = std::is_same<smop::DateTime::period, std::nano>::value ? "ns" : 
        (std::is_same<smop::DateTime::period, std::micro>::value ? "us" : 
        (std::is_same<smop::DateTime::period, std::milli>::value ? "ms" : ""));
    return o << dur.count() << unit;
}

inline std::ostream& operator<<(std::ostream& o, smop::DateTime dt)
{
    return o << dt.time_since_epoch() << " + epoch";
}

template<typename T>
inline std::ostream& operator<<(std::ostream& o, const std::vector<T>& vec)
{
    o << "std::vector { size: " << vec.size();
    for (const auto& elem : vec)
    {
        o << ", " << elem;
    }
    return o << " }";
}
