#include <vector>
#include <points.hpp>

#include "algorithms.hpp"

namespace simplify::algorithms
{
    double squareDistance(const IPoint &left, const IPoint &right)
    {
        auto delta = left - right;

        auto x = delta.x;
        auto y = delta.y;
        auto z = delta.z;

        return x * x + y * y + z * z;
    }

    double segmentSquareDistance(const IPoint &point, const IPoint &segmentStart, const IPoint &segmentEnd)
    {
        auto pointCoords = point.getCoordinates();
        auto delta = segmentEnd - segmentStart;

        auto result = [pointCoords](double x, double y, double z)
        {
            auto dx = pointCoords.x - x;
            auto dy = pointCoords.y - y;
            auto dz = pointCoords.z - z;
            return dx * dx + dy * dy + dz * dz;
        };

        if (delta.x != 0 || delta.y != 0 || delta.z != 0)
        {
            auto t = ((pointCoords.x - segmentStart.getX()) * delta.x +
                      (pointCoords.y - segmentStart.getY()) * delta.y +
                      (pointCoords.z - segmentStart.getZ()) * delta.z) /
                     (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

            if (t > 1)
            {
                return result(segmentEnd.getX(), segmentEnd.getY(), segmentEnd.getZ());
            }
            else
            {
                return result(segmentStart.getX() + t * delta.x,
                              segmentStart.getY() + t * delta.y,
                              segmentStart.getZ() + t * delta.z);
            }
        }
        else
        {
            return result(segmentStart.getX(), segmentStart.getY(), segmentStart.getZ());
        }
    }

    std::vector<IPoint *> simplifyRadialDistance(const std::vector<IPoint *> &points, const double tolerance)
    {
        if (points.size() <= 1)
        {
            return points;
        }

        std::vector<IPoint *> simplifiedPoints;
        auto previous = points.front();
        simplifiedPoints.push_back(previous);
        IPoint *current = nullptr;

        for (size_t i = 1; i < points.size(); ++i)
        {
            current = points[i];
            if (squareDistance(*previous, *current) > tolerance)
            {
                simplifiedPoints.push_back(current);
                previous = current;
            }
        }

        if (current != previous)
        {
            simplifiedPoints.push_back(current);
        }

        return simplifiedPoints;
    }

    std::vector<IPoint *> simplifyDouglasPeucker(const std::vector<IPoint *> &points, const double tolerance)
    {
        if (points.size() <= 1)
        {
            return points;
        }

        auto first = points.front();
        auto last = points.back();

        auto foundIndex = 0;
        auto bestDistance = 0.0;

        for (size_t i = 1; i < points.size() - 1; ++i)
        {
            auto point = points[i];
            if (point == first || point == last)
            {
                continue;
            }

            auto distance = segmentSquareDistance(*point, *first, *last);
            if (distance >= bestDistance)
            {
                bestDistance = distance;
                foundIndex = i;
            }
        }

        if (bestDistance > tolerance)
        {
            auto left = std::vector<IPoint *>(points.begin(), points.begin() + foundIndex + 1);
            auto right = std::vector<IPoint *>(points.begin() + foundIndex, points.end());

            auto leftSimplified = simplifyDouglasPeucker(left, tolerance);
            auto rightSimplified = simplifyDouglasPeucker(right, tolerance);

            leftSimplified.pop_back();
            leftSimplified.insert(leftSimplified.end(), rightSimplified.begin(), rightSimplified.end());

            return leftSimplified;
        }

        return {first, last};
    }
}
