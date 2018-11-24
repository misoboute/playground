#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

using testing::Test;

int solution(const std::string &S, const std::string &T, const std::string &U) {
    auto catalog = std::unordered_map< std::string, std::vector<size_t> >();
    auto catalogStr = std::istringstream(S);
    while (catalogStr.good())
    {
        size_t version = 0, size = 0;
        std::string region, versionStr, sizeStr;
        std::getline(catalogStr, region, ',');
        std::getline(catalogStr, versionStr, ',');
        std::getline(catalogStr, sizeStr);
        std::istringstream(versionStr) >> version;
        std::istringstream(sizeStr) >> size;
        catalog[region].push_back(size);
    }

    auto installed = std::unordered_map<std::string, size_t>();
    auto installedStream = std::istringstream(T);
    while (installedStream.good())
    {
        size_t version = 0;
        std::string region, versionStr;
        std::getline(installedStream, region, ',');
        std::getline(installedStream, versionStr);
        std::istringstream(versionStr) >> version;
        installed[region] = version;
    }

    auto regionSpecsItr = catalog.find(U);
    if (regionSpecsItr == catalog.cend())
        return -1;

    const auto& regionSpecs = regionSpecsItr->second;
    size_t startIndex = 0;
    auto latestInstalledVerItr = installed.find(U);
    if (latestInstalledVerItr != installed.cend())
        startIndex = latestInstalledVerItr->second;

    size_t totalSize = 0;
    for (auto i = startIndex; i < regionSpecs.size(); ++i)
        totalSize += regionSpecs[i];

    return totalSize;
}
struct TomTomFixture : public Test
{
    TomTomFixture()
    {
    }

    ~TomTomFixture()
    {
    }
};

TEST_F(TomTomFixture, T1)
{
    EXPECT_EQ(
        solution("Netherlands,1,25000\nNetherlands,2,3000\nNetherlands,3,1000\n",
            "Netherlands,1", "Netherlands"),
        4000);
}

TEST_F(TomTomFixture, T2)
{
    EXPECT_EQ(
        solution("Netherlands,1,25000\nNetherlands,2,3000\nNetherlands,3,1000\n",
            "Netherlands,1", "United Kingdom"),
        -1);
}
