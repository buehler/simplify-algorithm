package dev.cbue.simplify.models;

public record Point3d(double x, double y, double z) implements Point {
    @Override
    public Coordinates getCoordinates() {
        return new Coordinates(x, y, z);
    }
}
