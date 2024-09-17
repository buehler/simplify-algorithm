package dev.cbue.simplify.testData;

import dev.cbue.simplify.models.Point2d;

public record SegmentDistance(Point2d point, Point2d start, Point2d end, double distance) {
}
