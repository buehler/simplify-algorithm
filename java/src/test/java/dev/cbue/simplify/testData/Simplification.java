package dev.cbue.simplify.testData;

import dev.cbue.simplify.models.Point2d;

public record Simplification(double tolerance, Point2d[] result) {
}
