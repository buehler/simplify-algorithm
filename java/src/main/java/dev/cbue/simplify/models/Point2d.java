package dev.cbue.simplify.models;

public record Point2d(double x, double y) implements Point {
    @Override
    public Coordinates getCoordinates() {
        return new Coordinates(x, y, 0);
    }
}
