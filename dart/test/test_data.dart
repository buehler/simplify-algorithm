import 'dart:convert';
import 'dart:io';

import 'package:simplify_dart/simplify.dart';

class _PointDistance {
  final Point left;
  final Point right;
  final num distance;

  _PointDistance.fromJson(Map<String, dynamic> json)
      : distance = json['distance'],
        left = Point2d(json['left']['x'], json['left']['y']),
        right = Point2d(json['right']['x'], json['right']['y']);
}

class _SegmentDistance {
  final Point point;
  final Point start;
  final Point end;
  final num distance;

  _SegmentDistance.fromJson(Map<String, dynamic> json)
      : distance = json['distance'],
        point = Point2d(json['point']['x'], json['point']['y']),
        start = Point2d(json['start']['x'], json['start']['y']),
        end = Point2d(json['end']['x'], json['end']['y']);
}

class _AlgorithmicSimplification {
  final num tolerance;
  final bool highQuality;
  final List<Point> result;

  _AlgorithmicSimplification.fromJson(Map<String, dynamic> json)
      : tolerance = json['tolerance'],
        highQuality =
            json.containsKey('highQuality') ? json['highQuality'] : false,
        result = (json['result'] as List<dynamic>)
            .map((e) => Point2d(e['x'], e['y']))
            .toList();
}

List<_PointDistance> getDistances() {
  final content = File('../test-data/point-distances.json').readAsStringSync();
  final json = jsonDecode(content);
  final result = <_PointDistance>[];

  for (var object in json) {
    result.add(_PointDistance.fromJson(object));
  }

  return result;
}

List<_SegmentDistance> getSegmentDistances() {
  final content =
      File('../test-data/segment-distances.json').readAsStringSync();
  final json = jsonDecode(content);
  final result = <_SegmentDistance>[];

  for (var object in json) {
    result.add(_SegmentDistance.fromJson(object));
  }

  return result;
}

List<Point> getPoints() {
  final content = File('../test-data/points.json').readAsStringSync();
  final json = jsonDecode(content);
  final result = <Point>[];

  for (var object in json) {
    result.add(Point2d(object['x'], object['y']));
  }

  return result;
}

List<_AlgorithmicSimplification> getRadialSimplifications() {
  final content =
      File('../test-data/radial-simplifications.json').readAsStringSync();
  final json = jsonDecode(content);
  final result = <_AlgorithmicSimplification>[];

  for (var object in json) {
    result.add(_AlgorithmicSimplification.fromJson(object));
  }

  return result;
}

List<_AlgorithmicSimplification> getDouglasPeuckerSimplifications() {
  final content = File('../test-data/douglas-peucker-simplifications.json')
      .readAsStringSync();
  final json = jsonDecode(content);
  final result = <_AlgorithmicSimplification>[];

  for (var object in json) {
    result.add(_AlgorithmicSimplification.fromJson(object));
  }

  return result;
}

List<_AlgorithmicSimplification> getAlgorithmicSimplifications() {
  final content =
      File('../test-data/algorithm-simplifications.json').readAsStringSync();
  final json = jsonDecode(content);
  final result = <_AlgorithmicSimplification>[];

  for (var object in json) {
    result.add(_AlgorithmicSimplification.fromJson(object));
  }

  return result;
}
