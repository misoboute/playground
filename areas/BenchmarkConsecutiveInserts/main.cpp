#include <chrono>
#include <iostream>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

int main(int, char**)
{
    const auto maxInt = std::numeric_limits<int>::max();
    const auto minInt = std::numeric_limits<int>::min();
    const auto benchmarkSize = 2000000;
    auto randEng = std::default_random_engine();
    auto dist = std::uniform_int_distribution<int>(minInt, maxInt);

    std::set<int> mySet;
    std::unordered_set<int> myUnorderedSet;
    std::vector<int> myVec;
    
    const auto checkPoint1 = high_resolution_clock::now();

    for (int i = 0; i < benchmarkSize; ++i)
    {
        mySet.emplace(dist(randEng));
    }
    
    const auto checkPoint2 = high_resolution_clock::now();

    for (int i = 0; i < benchmarkSize; ++i)
    {
        myUnorderedSet.emplace(dist(randEng));
    }
    
    const auto checkPoint3 = high_resolution_clock::now();

    for (int i = 0; i < benchmarkSize; ++i)
    {
        myVec.push_back(dist(randEng));
    }
    
    const auto checkPoint4 = high_resolution_clock::now();
    
    const auto setTime = 
        duration_cast<milliseconds>(checkPoint2 - checkPoint1);
    const auto unorderedSetTime = 
        duration_cast<milliseconds>(checkPoint3 - checkPoint2);
    const auto vecTime = 
        duration_cast<milliseconds>(checkPoint4 - checkPoint3);
    
    std::cout << "std::set took " << setTime.count() << " ms\n";
    std::cout << "std::unordered_set took " << unorderedSetTime.count() << " ms\n";
    std::cout << "std::vector took " << vecTime.count() << " ms\n";
    
    return 0;
}
