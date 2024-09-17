package dev.cbue.simplify.testData;

import dev.cbue.simplify.models.Point2d;

public record AlgorithmicSimplification(boolean highQuality, double tolerance, Point2d[] result) {
}
