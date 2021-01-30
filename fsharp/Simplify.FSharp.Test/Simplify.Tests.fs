module Tests

open Simplify
open TestData
open Xunit

let floatMargin = 0.000001

[<Fact>]
let ``Should return empty list`` () =
    Assert.Equal<List<Point>>([], Simplify.simplify [] 1.0 false)

[<Fact>]
let ``Should return list with one point`` () =
    let p = [Point2d(1.0, 2.0)]
    Assert.Equal<List<Point>>(p, Simplify.simplify p 1.0 false)

[<Fact>]
let ``Should return list with two points`` () =
    let p = [Point2d(0.0, 0.0); Point2d(1.0, 2.0)]
    Assert.Equal<List<Point>>(p, Simplify.simplify p 1.0 false)

[<Theory>]
[<MemberData("distances", MemberType = typeof<TestData>)>]
let ``Should correctly calculate the square distance`` ((left, right, distance): Distance): unit =
    Assert.InRange(Simplify.squareDistance left right, distance - floatMargin, distance + floatMargin)

[<Theory>]
[<MemberData("segmentDistances", MemberType = typeof<TestData>)>]
let ``Should correctly calculate the square distance from a point to a segment`` ((point, start, ``end``, distance): SegmentDistance) =
    Assert.InRange(Simplify.segmentSquareDistance point start ``end``, distance - floatMargin, distance + floatMargin)

[<Theory>]
[<MemberData("radialSimplifications", MemberType = typeof<TestData>)>]
let ``Should correctly simplify via radial distance`` ((tolerance, result): RadialSimplification) =
    let squaredTolerance = tolerance * tolerance
    Assert.Equal<List<Point>>(result, Simplify.simplifyRadialDistance TestData.points squaredTolerance)

[<Theory>]
[<MemberData("douglasPeuckerSimplifications", MemberType = typeof<TestData>)>]
let ``Should correctly simplify via douglas-peucker algorithm`` ((tolerance, result): DouglasPeuckerSimplification) =
    let squaredTolerance = tolerance * tolerance
    Assert.Equal<List<Point>>(result, Simplify.simplifyDouglasPeucker TestData.points squaredTolerance)

[<Theory>]
[<MemberData("simplifySimplifications", MemberType = typeof<TestData>)>]
let ``Should correctly simplify via simplify method`` ((highQuality, tolerance, result): SimplifySimplification) =
    Assert.Equal<List<Point>>(result, Simplify.simplify TestData.points tolerance highQuality)
