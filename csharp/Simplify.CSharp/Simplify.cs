using System;
using System.Collections.Generic;
using System.Linq;

namespace Simplify
{
    public static class Simplifier
    {
        internal static double SquareDistance(IPoint left, IPoint right)
        {
            var (leftX, leftY, leftZ) = left.Coordinates;
            var (rightX, rightY, rightZ) = right.Coordinates;

            return Math.Pow(leftX - rightX, 2) + Math.Pow(leftY - rightY, 2) + Math.Pow(leftZ - rightZ, 2);
        }

        internal static double SegmentSquareDistance(IPoint point, IPoint start, IPoint end)
        {
            var (x, y, z) = point.Coordinates;
            var (startX, startY, startZ) = start.Coordinates;
            var (endX, endY, endZ) = end.Coordinates;

            var deltaX = endX - startX;
            var deltaY = endY - startY;
            var deltaZ = endZ - startZ;

            double Result(double rx, double ry, double rz)
            {
                var dx = x - rx;
                var dy = y - ry;
                var dz = z - rz;
                return dx * dx + dy * dy + dz * dz;
            }

            if (deltaX == 0 && deltaY == 0 && deltaZ == 0)
            {
                return Result(startX, startY, startZ);
            }

            var t = ((x - startX) * deltaX + (y - startY) * deltaY + (z - startZ) * deltaZ) /
                    (deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

            return t > 1
                ? Result(endX, endY, endZ)
                : Result(startX + deltaX * t, startY + deltaY * t, startZ + deltaZ * t);
        }

        internal static IList<IPoint> SimplifyRadialDistance(IList<IPoint> points, double tolerance)
        {
            if (points.Count <= 2)
            {
                return points;
            }

            var previous = points.First();
            var newPoints = new List<IPoint> {previous};
            IPoint? selectedPoint = null;

            foreach (var point in points.Skip(1))
            {
                selectedPoint = point;

                if (!(SquareDistance(selectedPoint, previous) > tolerance))
                {
                    continue;
                }

                newPoints.Add(selectedPoint);
                previous = selectedPoint;
            }

            if (selectedPoint != null && previous != selectedPoint)
            {
                newPoints.Add(selectedPoint);
            }

            return newPoints;
        }

        internal static IList<IPoint> SimplifyDouglasPeucker(IList<IPoint> points, double tolerance)
        {
            if (points.Count <= 2)
            {
                return points;
            }

            var first = points.First();
            var last = points.Last();

            var (index, distance) = points
                .Select((point, i) => (
                    Index: i,
                    Distance: point == first || point == last ? -1 : SegmentSquareDistance(point, first, last)))
                .OrderByDescending(data => data.Distance)
                .First();

            if (distance > tolerance)
            {
                return SimplifyDouglasPeucker(points.Take(index + 1).ToList(), tolerance)
                    .SkipLast(1)
                    .Concat(SimplifyDouglasPeucker(points.Skip(index).ToList(), tolerance))
                    .ToList();
            }

            return new[] {first, last};
        }

        public static IEnumerable<IPoint> Simplify(IEnumerable<IPoint> points, double tolerance,
            bool highQuality = false)
        {
            var pointList = points.ToList();
            if (pointList.Count <= 2)
            {
                return pointList;
            }

            var squaredTolerance = tolerance * tolerance;

            return highQuality
                ? SimplifyDouglasPeucker(pointList, squaredTolerance)
                : SimplifyDouglasPeucker(SimplifyRadialDistance(pointList, squaredTolerance), squaredTolerance);
        }
    }
}
