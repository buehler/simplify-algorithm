import 'dart:math';

import 'points.dart';

num squareDistance(Point left, Point right) =>
    pow(left.x - right.x, 2) + pow(left.y - right.y, 2) + pow(left.z - right.z, 2);

num segmentSquareDistance(Point point, Point start, Point end) {
  final x = point.x;
  final y = point.y;
  final z = point.z;

  final startX = start.x;
  final startY = start.y;
  final startZ = start.z;

  final endX = end.x;
  final endY = end.y;
  final endZ = end.z;

  final deltaX = endX - startX;
  final deltaY = endY - startY;
  final deltaZ = endZ - startZ;

  num result(num rx, num ry, num rz) => pow(x - rx, 2) + pow(y - ry, 2) + pow(z - rz, 2);

  if (deltaX == 0 && deltaY == 0 && deltaZ == 0) {
    return result(startX, startY, startZ);
  }

  final t = ((x - startX) * deltaX + (y - startY) * deltaY + (z - startZ) * deltaZ) /
      (deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

  return t > 1 ? result(endX, endY, endZ) : result(startX + deltaX * t, startY + deltaY * t, startZ + deltaZ * t);
}

List<Point> simplifyRadialDistance(List<Point> points, num tolerance) {
  if (points.length <= 2) {
    return points;
  }

  var previous = points.first;
  final newPoints = [previous];
  Point? selectedPoint;

  for (var point in points.skip(1)) {
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

List<Point> simplifyDouglasPeucker(List<Point> points, num tolerance) {
  if (points.length <= 2) {
    return points;
  }

  var first = points.first;
  var last = points.last;

  var foundIndex = 0;
  num bestDistance = 0;

  for (var point in points) {
    if (point == first || point == last) {
      continue;
    }

    var distance = segmentSquareDistance(point, first, last);
    if (distance >= bestDistance) {
      bestDistance = distance;
      foundIndex = points.indexOf(point);
    }
  }

  if (bestDistance > tolerance) {
    var leftPart = points.sublist(0, foundIndex + 1);
    var rightPart = points.sublist(foundIndex);

    var left = simplifyDouglasPeucker(leftPart, tolerance);
    left = left.take(left.length - 1).toList();

    var right = simplifyDouglasPeucker(rightPart, tolerance);

    return [...left, ...right];
  }

  return [first, last];
}
