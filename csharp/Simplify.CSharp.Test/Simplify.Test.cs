using System.Collections.Generic;
using System.Linq;
using Xunit;

namespace Simplify.CSharp.Test
{
    public class SimplifyTest
    {
        [Fact]
        public void Should_Return_Empty_list() =>
            Assert.Equal(new List<IPoint>(), Simplifier.Simplify(new List<IPoint>(), 1));

        [Fact]
        public void Should_Return_List_With_One_Point()
        {
            var l = new List<IPoint> {new Point2d(1, 2)};
            Assert.Equal(l, Simplifier.Simplify(l, 1));
        }

        [Fact]
        public void Should_Return_List_With_Two_Point()
        {
            var l = new List<IPoint> {new Point2d(1, 2), new Point2d(0, 0)};
            Assert.Equal(l, Simplifier.Simplify(l, 1));
        }

        [Theory]
        [MemberData(nameof(TestData.Distances), MemberType = typeof(TestData))]
        public void Should_Correctly_Calculate_Squared_Distance(IPoint left, IPoint right, double distance) =>
            Assert.Equal(Simplifier.SquareDistance(left, right), distance, 8);

        [Theory]
        [MemberData(nameof(TestData.SegmentDistances), MemberType = typeof(TestData))]
        public void Should_Correctly_Calculate_Segment_Squared_Distance(IPoint point, IPoint start, IPoint end,
            double distance) =>
            Assert.Equal(Simplifier.SegmentSquareDistance(point, start, end), distance, 8);

        [Theory]
        [MemberData(nameof(TestData.RadialSimplifications), MemberType = typeof(TestData))]
        public void Should_Correctly_Simplify_Via_Radial_Distance(double tolerance, IEnumerable<IPoint> result) =>
            Assert.Equal(result, Simplifier.SimplifyRadialDistance(TestData.Points.ToList(), tolerance * tolerance));

        [Theory]
        [MemberData(nameof(TestData.DouglasPeukerSimplifications), MemberType = typeof(TestData))]
        public void Should_Correctly_Simplify_Via_DouglasPeucker_Algorithm(double tolerance,
            IEnumerable<IPoint> result) =>
            Assert.Equal(result, Simplifier.SimplifyDouglasPeucker(TestData.Points.ToList(), tolerance * tolerance));

        [Theory]
        [MemberData(nameof(TestData.AlgorithmicSimplifications), MemberType = typeof(TestData))]
        public void Should_Correctly_Simplify_Via_Simplify_Method(bool highQuality, double tolerance,
            IEnumerable<IPoint> result) =>
            Assert.Equal(result, Simplifier.Simplify(TestData.Points.ToList(), tolerance, highQuality));
    }
}
