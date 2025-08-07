#pragma once
#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_SIMPLIFY_ALGORITHM
#define SIMPLIFY_ALGORITHM_PUBLIC __declspec(dllexport)
#else
#define SIMPLIFY_ALGORITHM_PUBLIC __declspec(dllimport)
#endif
#else
#ifdef BUILDING_SIMPLIFY_ALGORITHM
#define SIMPLIFY_ALGORITHM_PUBLIC __attribute__((visibility("default")))
#else
#define SIMPLIFY_ALGORITHM_PUBLIC
#endif
#endif

namespace simplify
{
    struct SIMPLIFY_ALGORITHM_PUBLIC Coordinates
    {
        double x;
        double y;
        double z;

        Coordinates(double x, double y, double z) : x(x), y(y), z(z) {}

        bool operator==(const Coordinates &other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    class SIMPLIFY_ALGORITHM_PUBLIC IPoint
    {
    public:
        static IPoint *create(double x, double y);
        static IPoint *create(double x, double y, double z);

        virtual double getX() const = 0;
        virtual double getY() const = 0;
        virtual double getZ() const = 0;

        bool operator==(const IPoint &other) const
        {
            return getX() == other.getX() && getY() == other.getY() && getZ() == other.getZ();
        }

        Coordinates operator-(const IPoint &other) const
        {
            return Coordinates(getX() - other.getX(), getY() - other.getY(), getZ() - other.getZ());
        }

        Coordinates getCoordinates() const
        {
            return Coordinates(getX(), getY(), getZ());
        }

        virtual ~IPoint() = default;
    };

    class SIMPLIFY_ALGORITHM_PUBLIC Point2D : public IPoint
    {
    private:
        double x;
        double y;

    public:
        Point2D(double x, double y) : x(x), y(y) {}

        double getX() const override { return x; }
        double getY() const override { return y; }
        double getZ() const override { return 0; }
    };

    class SIMPLIFY_ALGORITHM_PUBLIC Point3D : public Point2D
    {
    private:
        double z;

    public:
        Point3D(double x, double y, double z) : Point2D(x, y), z(z) {}

        double getZ() const override { return z; }
    };
}