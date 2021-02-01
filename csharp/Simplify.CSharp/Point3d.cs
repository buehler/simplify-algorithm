namespace Simplify
{
    public record Point3d : IPoint
    {
        public Point3d(double x, double y, double z) => Coordinates = (x, y, z);

        public (double X, double Y, double Z) Coordinates { get; }

        public static implicit operator Point2d(Point3d point) => new (point.Coordinates.X, point.Coordinates.Y);
    }
}
