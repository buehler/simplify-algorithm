abstract class Point {
  final num x;
  final num y;
  final num z;

  Point(this.x, this.y, this.z);

  @override
  String toString() => 'Point(x: $x, y: $y, z: $z)';
}

class Point2d extends Point {
  Point2d(num x, num y) : super(x, y, 0);
}

class Point3d extends Point {
  Point3d(num x, num y, num z) : super(x, y, z);
}
