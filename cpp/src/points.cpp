#include <points.hpp>

namespace simplify
{
    IPoint *IPoint::create(double x, double y)
    {
        return new Point2D(x, y);
    }

    IPoint *IPoint::create(double x, double y, double z)
    {
        return new Point3D(x, y, z);
    }
}
