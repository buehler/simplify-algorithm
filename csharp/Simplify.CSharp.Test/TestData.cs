using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;

namespace Simplify.CSharp.Test
{
    public static class TestData
    {
        public static IEnumerable<IPoint> Points
            => new List<IPoint>(
                LoadJson<JsonPoint>("points.json")
                    .Select(o => new Point2d(o.X, o.Y)));

        public static IEnumerable<object[]> Distances
            => new List<object[]>(
                LoadJson<JsonPointDistance>("point-distances.json")
                    .Select(o => new object[] {o.Left, o.Right, o.Distance}));

        public static IEnumerable<object[]> SegmentDistances
            => new List<object[]>(
                LoadJson<JsonSegmentDistance>("segment-distances.json")
                    .Select(o => new object[] {o.Point, o.Start, o.End, o.Distance}));

        public static IEnumerable<object[]> RadialSimplifications
            => new List<object[]>(
                LoadJson<JsonSimplification>("radial-simplifications.json")
                    .Select(o => new object[] {o.Tolerance, o.Result}));

        public static IEnumerable<object[]> DouglasPeukerSimplifications
            => new List<object[]>(
                LoadJson<JsonSimplification>("douglas-peucker-simplifications.json")
                    .Select(o => new object[] {o.Tolerance, o.Result}));

        public static IEnumerable<object[]> AlgorithmicSimplifications
            => new List<object[]>(
                LoadJson<JsonAlgorithmicSimplification>("algorithm-simplifications.json")
                    .Select(o => new object[] {o.HighQuality, o.Tolerance, o.Result}));

        private static IEnumerable<TResult> LoadJson<TResult>(string fileName)
        {
            var json = File.ReadAllText(Path.Join("Data", fileName));
            return JsonConvert.DeserializeObject<IEnumerable<TResult>>(json);
        }

        private record JsonPoint(double X, double Y);

        private record JsonPointDistance(Point2d Left, Point2d Right, double Distance);

        private record JsonSegmentDistance(Point2d Point, Point2d Start, Point2d End, double Distance);

        private record JsonSimplification(double Tolerance, IEnumerable<Point2d> Result);

        private record JsonAlgorithmicSimplification(bool HighQuality, double Tolerance, IEnumerable<Point2d> Result);
    }
}
