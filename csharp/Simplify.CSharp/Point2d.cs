namespace Simplify
{
    public record Point2d : IPoint
    {
        public Point2d(double x, double y) => Coordinates = (x, y, 0);

        public (double X, double Y, double Z) Coordinates { get; }

        public static implicit operator Point3d(Point2d point) => new(point.Coordinates.X, point.Coordinates.Y, 0);
    }
}
