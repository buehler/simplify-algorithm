#pragma once
#include <vector>
#include <points.hpp>

namespace simplify::algorithms
{
    double squareDistance(const IPoint &left, const IPoint &right);
    double segmentSquareDistance(const IPoint &point, const IPoint &segmentStart, const IPoint &segmentEnd);
    std::vector<IPoint *> simplifyRadialDistance(const std::vector<IPoint *> &points, const double tolerance);
    std::vector<IPoint *> simplifyDouglasPeucker(const std::vector<IPoint *> &points, const double tolerance);
}
