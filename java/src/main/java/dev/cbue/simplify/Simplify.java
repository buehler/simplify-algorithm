package dev.cbue.simplify;

import dev.cbue.simplify.models.Coordinates;
import dev.cbue.simplify.models.Point;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.TreeMap;

public class Simplify {
    static double squareDistance(Point left, Point right) {
        if (left.getCoordinates() instanceof Coordinates(
                double lx, double ly, double lz
        ) && right.getCoordinates() instanceof Coordinates(double rx, double ry, double rz)) {
            return Math.pow(lx - rx, 2) + Math.pow(ly - ry, 2) + Math.pow(lz - rz, 2);
        }

        return 0;
    }

    static double segmentSquareDistance(Point point, Point start, Point end) {
        if (start.getCoordinates() instanceof Coordinates(
                double sx, double sy, double sz
        ) && end.getCoordinates() instanceof Coordinates(
                double ex, double ey, double ez
        ) && point.getCoordinates() instanceof Coordinates(double px, double py, double pz)) {
            var deltaX = ex - sx;
            var deltaY = ey - sy;
            var deltaZ = ez - sz;

            record Result(double x, double y, double z) {
                double getResult(double rx, double ry, double rz) {
                    var dx = x - rx;
                    var dy = y - ry;
                    var dz = z - rz;
                    return dx * dx + dy * dy + dz * dz;
                }
            }

            var r = new Result(px, py, pz);

            if (deltaX == 0 && deltaY == 0 && deltaZ == 0) {
                return r.getResult(sx, sy, sz);
            }

            var t = ((px - sx) * deltaX + (py - sy) * deltaY + (pz - sz) * deltaZ) / (deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

            return t > 1 ? r.getResult(ex, ey, ez) : r.getResult(sx + deltaX * t, sy + deltaY * t, sz + deltaZ * t);
        }

        return 0;
    }

    static List<Point> simplifyDouglasPeucker(List<Point> points, double tolerance) {
        if (points.size() <= 2) {
            return points;
        }

        var first = points.getFirst();
        var last = points.getLast();

        var map = new TreeMap<Double, Integer>();
        var idx = 0;
        for (var point : points) {
            var distance = point == first || point == last
                    ? -1
                    : segmentSquareDistance(point, first, last);
            map.put(distance, idx++);
        }

        var lastEntry = map.lastEntry();
        var lastDistance = lastEntry.getKey();
        var lastIdx = lastEntry.getValue();

        if (lastDistance > tolerance) {
            var firstHalf = simplifyDouglasPeucker(points.subList(0, lastIdx), tolerance);
            firstHalf.removeLast();
            var secondHalf = simplifyDouglasPeucker(points.subList(lastIdx, points.size()), tolerance);

            firstHalf.addAll(secondHalf);
            return firstHalf;
        }

        return Arrays.asList(first, last);
    }

    static List<Point> simplifyRadialDistance(List<Point> points, double tolerance) {
        if (points.size() <= 2) {
            return points;
        }

        var previous = points.getFirst();
        var newPoints = new ArrayList<Point>();
        newPoints.add(previous);

        Point selectedPoint = null;

        for (var point : points.subList(1, points.size())) {
            selectedPoint = point;

            if (!(squareDistance(selectedPoint, previous) > tolerance)) {
                continue;
            }

            newPoints.add(selectedPoint);
            previous = selectedPoint;
        }

        if (selectedPoint != null && previous != selectedPoint) {
            newPoints.add(selectedPoint);
        }

        return newPoints;
    }

    public static List<Point> simplify(Iterable<Point> points, double tolerance) {
        return simplify(points, tolerance, false);
    }

    public static List<Point> simplify(Iterable<Point> points, double tolerance, boolean highQuality) {
        var list = new ArrayList<Point>();
        points.forEach(list::add);

        if (list.size() <= 2) {
            return list;
        }

        var sqTolerance = tolerance * tolerance;
        return highQuality
                ? simplifyDouglasPeucker(list, sqTolerance)
                : simplifyDouglasPeucker(simplifyRadialDistance(list, sqTolerance), sqTolerance);
    }
}
