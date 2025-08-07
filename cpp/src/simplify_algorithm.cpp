#include <simplify_algorithm.hpp>
#include <algorithms.hpp>

namespace simplify
{
  std::vector<IPoint *> simplify(const std::vector<IPoint *> &points, const double tolerance, const bool highQuality)
  {
    if (points.size() <= 2)
    {
      return points;
    }

    auto squaredTolerance = tolerance * tolerance;

    if (highQuality)
    {
      return algorithms::simplifyDouglasPeucker(points, squaredTolerance);
    }

    return algorithms::simplifyDouglasPeucker(
        algorithms::simplifyRadialDistance(points, squaredTolerance),
        squaredTolerance);
  }
}
