package dev.cbue.simplify;


import dev.cbue.simplify.models.Point;
import dev.cbue.simplify.models.Point2d;
import dev.cbue.simplify.testData.*;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class SimplifyTest {
    @ParameterizedTest
    @MethodSource("dev.cbue.simplify.testData.TestData#pointDistances")
    public void squaredDistance_correctlyCalculate(PointDistance testCase) {
        var result = Simplify.squareDistance(testCase.left(), testCase.right());
        assertEquals(testCase.distance(), result);
    }

    @ParameterizedTest
    @MethodSource("dev.cbue.simplify.testData.TestData#segmentDistances")
    public void segmentDistance_correctlyCalculate(SegmentDistance testCase) {
        var result = Simplify.segmentSquareDistance(testCase.point(), testCase.start(), testCase.end());
        assertEquals(testCase.distance(), result);
    }

    @Test
    public void emptyList() {
        var points = new ArrayList<Point>();
        var result = Simplify.simplify(points, 1, false);
        assertEquals(points, result);
    }

    @Test
    public void list_withOneItem() {
        var points = new ArrayList<Point>();
        points.add(new Point2d(0, 0));
        var result = Simplify.simplify(points, 1, false);
        assertEquals(points, result);
    }

    @Test
    public void list_withTwoItems() {
        var points = new ArrayList<Point>();
        points.add(new Point2d(0, 0));
        points.add(new Point2d(1, 1));
        var result = Simplify.simplify(points, 1, false);
        assertEquals(points, result);
    }

    @ParameterizedTest
    @MethodSource("dev.cbue.simplify.testData.TestData#radialSimplifications")
    public void simplifyRadialDistance(Simplification testCase) {
        var points = TestData.points();
        var tolerance = testCase.tolerance() * testCase.tolerance();
        var result = Simplify.simplifyRadialDistance(List.of(points), tolerance).toArray();
        assertArrayEquals(testCase.result(), result);
    }

    @ParameterizedTest
    @MethodSource("dev.cbue.simplify.testData.TestData#douglasPeuckerSimplifications")
    public void simplifyDouglasPeucker(Simplification testCase) {
        var points = TestData.points();
        var tolerance = testCase.tolerance() * testCase.tolerance();
        var result = Simplify.simplifyDouglasPeucker(List.of(points), tolerance).toArray();
        assertArrayEquals(testCase.result(), result);
    }

    @ParameterizedTest
    @MethodSource("dev.cbue.simplify.testData.TestData#algorithmicSimplifications")
    public void simplify(AlgorithmicSimplification testCase) {
        var points = TestData.points();
        var tolerance = testCase.tolerance() * testCase.tolerance();
        var result = Simplify.simplify(List.of(points), tolerance, testCase.highQuality()).toArray();
        assertArrayEquals(testCase.result(), result);
    }
}
