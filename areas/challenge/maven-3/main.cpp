#include <sstream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

using testing::Test;

struct Cell
{
    Cell() : x_(0), y_(0) { }

    Cell(const std::string& description)
    : x_(description[1] - 'A'), 
    y_(description[0] - '1')
    {
    }

    bool operator<= (const Cell& other) const
    {
        return x_ <= other.x_ && y_ <= other.y_;
    }

    int x_;
    int y_;
};

class Ship
{
public:
    Ship(const std::string& description)
    {
        std::istringstream parser(description);
        std::string topLeftStr, bottomRightStr;
        parser >> topLeftStr >> bottomRightStr;
        topLeft_ = Cell(topLeftStr);
        bottomRight_ = Cell(bottomRightStr);
        const auto height = bottomRight_.y_ - topLeft_.y_ + 1;
        const auto width = bottomRight_.x_ - topLeft_.x_ + 1;
        hitCells_ = std::vector< std::vector<bool> >(
            height, std::vector<bool>(width, false));
    }
    
    void NotifyHitCell(const Cell& cell)
    {
        if (topLeft_ <= cell && cell <= bottomRight_)
        {
            isHit_ = true;
            const auto y = cell.y_ - topLeft_.y_;
            const auto x = cell.x_ - topLeft_.x_;
            hitCells_[y][x] = true;
        }
    }
    
    bool IsHit() const
    {
        return isHit_;
    }
    
    bool IsSunken() const
    {
        for (const auto& row : hitCells_)
        {
            for (auto cell : row)
            {
                if (!cell)
                {
                    return false;
                }
            }
        }
        return true;
    }
    
private:
    Cell topLeft_;
    Cell bottomRight_;
    std::vector< std::vector<bool> > hitCells_;
    bool isHit_ = false;
};

std::string solution(int N, std::string &S, std::string &T)
{
    // write your code in C++14 (g++ 6.2.0)
    std::istringstream shipsParser(S);
    std::vector<Ship> ships;
    while (shipsParser.good())
    {
        std::string shipDesc;
        std::getline(shipsParser, shipDesc, ',');
        if (shipDesc.empty())
        {
            break;
        }
        ships.push_back(Ship(shipDesc));
    }
    
    std::istringstream hitParser(T);
    while (hitParser.good())
    {
        std::string hitDesc;
        hitParser >> hitDesc;
        if (hitDesc.empty())
        {
            break;
        }
        auto hitCell = Cell(hitDesc);
        for (auto& ship : ships)
        {
            ship.NotifyHitCell(hitCell);
        }
    }
    
    auto sunkenCount = 0, hitCount = 0;
    for (const auto ship : ships)
    {
        if (ship.IsSunken())
        {
            ++sunkenCount;
        }
        else if (ship.IsHit())
        {
            ++hitCount;
        }
    }
    
    std::ostringstream solutionStr;
    solutionStr << sunkenCount << "," << hitCount;
    return solutionStr.str();
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
    auto shipList = std::string("1B 2C,2D 4D");
    auto hitList = std::string("2B 2D 3D 4D 4A");
    EXPECT_EQ(solution(4, shipList, hitList), "1,1");
}

TEST_F(TomTomFixture, T2)
{
    auto shipList = std::string("1A 1B,2C 2C");
    auto hitList = std::string("1B");
    EXPECT_EQ(solution(3, shipList, hitList), "0,1");
}

TEST_F(TomTomFixture, T3)
{
    auto shipList = std::string("1A 2A,12A 12A");
    auto hitList = std::string("12A");
    EXPECT_EQ(solution(12, shipList, hitList), "1,0");
}

TEST_F(TomTomFixture, T4)
{
    auto shipList = std::string("");
    auto hitList = std::string("");
    EXPECT_EQ(solution(26, shipList, hitList), "0,0");
}

TEST_F(TomTomFixture, T5)
{
    auto shipList = std::string("");
    auto hitList = std::string("");
    EXPECT_EQ(solution(1, shipList, hitList), "0,0");
}

TEST_F(TomTomFixture, T6)
{
    auto shipList = std::string("1A 1A");
    auto hitList = std::string("");
    EXPECT_EQ(solution(1, shipList, hitList), "0,0");
}

TEST_F(TomTomFixture, T7)
{
    auto shipList = std::string("1A 1A");
    auto hitList = std::string("1A");
    EXPECT_EQ(solution(1, shipList, hitList), "1,0");
}
