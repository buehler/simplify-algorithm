import 'dart:math';

import 'package:simplify_dart/src/algorithms.dart';
import 'package:simplify_dart/src/points.dart';

List<Point> simplify(Iterable<Point> points, num tolerance,
    {bool highQuality = false}) {
  var pointList = points.toList();
  if (pointList.length <= 2) {
    return pointList;
  }

  var squaredTolerance = pow(tolerance, 2);

  return highQuality
      ? simplifyDouglasPeucker(pointList, squaredTolerance)
      : simplifyDouglasPeucker(
          simplifyRadialDistance(pointList, squaredTolerance),
          squaredTolerance);
}
