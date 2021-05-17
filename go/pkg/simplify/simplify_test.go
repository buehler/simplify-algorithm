package simplify

import (
	"testing"

	"github.com/go-test/deep"
)

func TestPoint2D_MapPoint(t *testing.T) {
	expectX, expectY, expectZ := 1.0, 2.0, 0.0

	point := Point2D{X: 1, Y: 2}
	resultX, resultY, resultZ := point.MapPoint()

	if expectX != resultX {
		t.Error("X != X")
	}

	if expectY != resultY {
		t.Error("Y != Y")
	}

	if expectZ != resultZ {
		t.Error("Z != Z")
	}
}

func TestPoint3D_MapPoint(t *testing.T) {
	expectX, expectY, expectZ := 1.0, 2.0, 3.0

	point := Point3D{X: 1, Y: 2, Z: 3}
	resultX, resultY, resultZ := point.MapPoint()

	if expectX != resultX {
		t.Error("X != X")
	}

	if expectY != resultY {
		t.Error("Y != Y")
	}

	if expectZ != resultZ {
		t.Error("Z != Z")
	}
}

func TestReturnEmptyList(t *testing.T) {
	expect := make([]MappablePoint, 0)
	result := Simplify(expect, 1, false)

	if diff := deep.Equal(expect, result); diff != nil {
		t.Error(diff)
	}
}

func TestReturnListWithOnePoint(t *testing.T) {
	var point MappablePoint
	point = &Point2D{X: 1, Y: 1}
	expect := []MappablePoint{point}
	result := Simplify(expect, 1, false)

	if diff := deep.Equal(expect, result); diff != nil {
		t.Error(diff)
	}
}

func TestReturnListWithTwoPoints(t *testing.T) {
	var point1 MappablePoint
	var point2 MappablePoint
	point1 = &Point2D{X: 1, Y: 1}
	point2 = &Point2D{X: 1, Y: 1}
	expect := []MappablePoint{point1, point2}
	result := Simplify(expect, 1, false)

	if diff := deep.Equal(expect, result); diff != nil {
		t.Error(diff)
	}
}

func TestCorrectlyCalculateSquaredDistance(t *testing.T) {
	cases := *pointDistances()

	for _, testCase := range cases {
		var left, right MappablePoint = &testCase.Left, &testCase.Right

		result := squareDistance(&left, &right)

		if result != testCase.Distance {
			t.Errorf("%f (result) was not %f (expected)", result, testCase.Distance)
		}
	}
}

func TestCorrectlyCalculateSegmentSquaredDistance(t *testing.T) {
	cases := *segmentDistances()

	for _, testCase := range cases {
		var point, start, end MappablePoint = &testCase.Point, &testCase.Start, &testCase.End

		result := segmentSquareDistance(&point, &start, &end)

		if result != testCase.Distance {
			t.Errorf("%f (result) was not %f (expected)", result, testCase.Distance)
		}
	}
}

func TestCorrectlySimplifyViaRadialDistance(t *testing.T) {
	cases := *radialSimplifications()

	for _, testCase := range cases {
		points := *points()
		tolerance := testCase.Tolerance * testCase.Tolerance
		expected := mapPoints(&testCase.Result)
		result := simplifyRadialDistance(points, tolerance)

		if diff := deep.Equal(*expected, result); diff != nil {
			t.Error(diff)
		}
	}
}

func TestCorrectlySimplifyViaDouglasPeuckerAlgorithm(t *testing.T) {
	cases := *douglasPeuckerSimplifications()

	for _, testCase := range cases {
		points := *points()
		tolerance := testCase.Tolerance * testCase.Tolerance
		expected := mapPoints(&testCase.Result)
		result := simplifyDouglasPeucker(points, tolerance)

		if diff := deep.Equal(*expected, result); diff != nil {
			t.Error(diff)
		}
	}
}

func TestCorrectlySimplifyViaSimplifyAlgorithm(t *testing.T) {
	cases := *algorithmicSimplifications()

	for _, testCase := range cases {
		points := *points()
		expected := mapPoints(&testCase.Result)
		result := Simplify(points, testCase.Tolerance, testCase.HighQuality)

		if diff := deep.Equal(*expected, result); diff != nil {
			t.Log(testCase)
			t.Error(diff)
		}
	}
}
