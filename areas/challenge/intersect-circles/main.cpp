#include <cmath>
#include <iostream>
#include <limits>

#include <gtest/gtest.h>

#include "RandomGenerator.h"

using testing::Test;

double solution(
    double x1, double y1, double r1, double x2, double y2, double r2)
{
    auto dSqr = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    auto d = std::sqrt(dSqr);

    if (d >= r1 + r2)
    {
        return 0;
    }

    const auto pi = 4 * std::atan(1);
    auto rM = std::max(r1, r2), rm = std::min(r1, r2);
    auto circM = pi * rM * rM, circm = pi * rm * rm;
    if (d + rm <= rM)
    {
        return circm;
    }
    if (d + rM <= rm)
    {
        return circM;
    }

    auto rMSqr = rM * rM, rmSqr = rm * rm;
    auto tSqr = dSqr + rMSqr - rmSqr;
    auto b = tSqr / 2 / d;
    auto lM = b, lm = d - b;
    auto cosAlphaM = lM / rM, cosAlpham = lm / rm;
    auto hSqr = rMSqr - b * b, h = std::sqrt(hSqr);
    auto triM = h * lM, trim = h * lm;
    auto interM = circM * std::acos(cosAlphaM) / pi - triM;
    auto interm = circm * std::acos(cosAlpham) / pi - trim;

    return interM + interm;
}

struct IntersectCirclesFixture : public Test
{
    IntersectCirclesFixture()
    {
        std::cout.precision(std::numeric_limits<long double>::digits10 + 1);
    }

    ~IntersectCirclesFixture()
    {
    }

    double solutionWithArbitraryCoordSys(double r1, double r2, double d)
    {
        auto x1 = rand_.Real(-50., 50.);
        auto y1 = rand_.Real(-50., 50.);
        auto x2 = rand_.Real(x1 - d, x1 + d);
        auto sign = rand_.Bool() ? -1 : 1;
        auto y2 = y1 + sign * std::sqrt(d * d - (x2 - x1) * (x2 - x1));
        return solution(10, 10, r1, 10, 10 + d, r2);
    }

    RandomGenerator rand_;
};

TEST_F(IntersectCirclesFixture, T1)
{
    EXPECT_EQ(solution(0, 0, 500, 0, 500, 500), 307092.4246521891328);
    std::cout << "0, 0, 500, 0, 500, 500 => " <<
        solution(0, 0, 500, 0, 500, 500) << std::endl;
}

TEST_F(IntersectCirclesFixture, Multi)
{
    for (double r1 = 1; r1 < 10; r1 += 2)
    {
        for (double r2 = 1; r2 < 10; r2 += 2)
        {
            for (double d = std::min(r1, r2) * .7; d < r1 + r2 + 1; d += .3)
            {
                std::cout << r1 << ", " << r2 << ", " << d << " => " <<
                    solutionWithArbitraryCoordSys(r1, r2, d) << std::endl;
            }
        }
    }

}
