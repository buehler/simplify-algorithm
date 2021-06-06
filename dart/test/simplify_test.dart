import 'package:simplify_dart/simplify.dart';
import 'package:simplify_dart/src/algorithms.dart';
import 'package:test/test.dart';

import 'test_data.dart';

void main() {
  group('Helper Algorithms', () {
    group('Squared Distance', () {
      final testCases = getDistances();
      testCases.forEach((testCase) {
        test('should correctly calculate distance ${testCase.distance}', () {
          final result = squareDistance(testCase.left, testCase.right);
          expect(result, equals(testCase.distance));
        });
      });
    });

    group('Squared Segment Distance', () {
      final testCases = getSegmentDistances();
      testCases.forEach((testCase) {
        test('should correctly calculate distance ${testCase.distance}', () {
          final result = segmentSquareDistance(
              testCase.point, testCase.start, testCase.end);
          expect(result, equals(testCase.distance));
        });
      });
    });

    group('Radial Simplification', () {
      final points = getPoints();
      final testCases = getRadialSimplifications();
      testCases.forEach((testCase) {
        test('should correctly calculate simplification ${testCase.tolerance}',
            () {
          final result = simplifyRadialDistance(
              points, testCase.tolerance * testCase.tolerance);

          for (var point in result) {
            final index = result.indexOf(point);
            expect(point.x, closeTo(testCase.result[index].x, 0.0001));
            expect(point.y, closeTo(testCase.result[index].y, 0.0001));
            expect(point.z, closeTo(testCase.result[index].z, 0.0001));
          }
        });
      });
    });

    group('Douglas Peucker Simplification', () {
      final points = getPoints();
      final testCases = getDouglasPeuckerSimplifications();
      testCases.forEach((testCase) {
        test('should correctly calculate simplification ${testCase.tolerance}',
            () {
          final result = simplifyDouglasPeucker(
              points, testCase.tolerance * testCase.tolerance);

          for (var point in result) {
            final index = result.indexOf(point);
            expect(point.x, closeTo(testCase.result[index].x, 0.0001));
            expect(point.y, closeTo(testCase.result[index].y, 0.0001));
            expect(point.z, closeTo(testCase.result[index].z, 0.0001));
          }
        });
      });
    });
  });

  group('Simplify Algorithm', () {
    test('should return empty list', () {
      expect(simplify([], 1), equals([]));
    });

    test('should return list with one point', () {
      final list = [Point2d(1, 2)];
      expect(simplify(list, 1), orderedEquals(list));
    });

    test('should return list with two points', () {
      final list = [Point2d(1, 2), Point2d(3, 4)];
      expect(simplify(list, 1), orderedEquals(list));
    });

    group('Algorithmic Simplification', () {
      final points = getPoints();
      final testCases = getAlgorithmicSimplifications();
      testCases.forEach((testCase) {
        test(
            'should correctly calculate simplification ${testCase.tolerance} / ${testCase.highQuality}',
            () {
          final result = simplify(points, testCase.tolerance,
              highQuality: testCase.highQuality);

          for (var point in result) {
            final index = result.indexOf(point);
            expect(point.x, closeTo(testCase.result[index].x, 0.0001));
            expect(point.y, closeTo(testCase.result[index].y, 0.0001));
            expect(point.z, closeTo(testCase.result[index].z, 0.0001));
          }
        });
      });
    });
  });
}
