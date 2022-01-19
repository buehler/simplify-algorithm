use crate::point::Point;

fn square_distance(left: &Point, right: &Point) -> f64 {
    let square_diff: fn((f64, f64, f64), (f64, f64, f64)) -> f64 = |(xl, yl, zl), (xr, yr, zr)| {
        (xl - xr).powf(2.0) + (yl - yr).powf(2.0) + (zl - zr).powf(2.0)
    };

    square_diff(left.map(), right.map())
}

fn segment_square_distance(point: &Point, start: &Point, end: &Point) -> f64 {
    let (x, y, z) = point.map();
    let (start_x, start_y, start_z) = start.map();
    let (end_x, end_y, end_z) = end.map();

    let delta_x = end_x - start_x;
    let delta_y = end_y - start_y;
    let delta_z = end_z - start_z;

    let result = |rx: f64, ry: f64, rz: f64| -> f64 {
        let dx = x - rx;
        let dy = y - ry;
        let dz = z - rz;
        dx.powf(2.0) + dy.powf(2.0) + dz.powf(2.0)
    };

    if delta_x != 0.0 || delta_y != 0.0 || delta_z != 0.0 {
        let t = ((x - start_x) * delta_x + (y - start_y) * delta_y + (z - start_z) * delta_z)
            / (delta_x.powf(2.0) + delta_y.powf(2.0) + delta_z.powf(2.0));
        if t > 1.0 {
            result(end_x, end_y, end_z)
        } else {
            result(
                start_x + delta_x * t,
                start_y + delta_y * t,
                start_z + delta_z * t,
            )
        }
    } else {
        result(start_x, start_y, start_z)
    }
}

pub(crate) fn simplify_radial_distance(points: &[Point], tolerance: f64) -> Vec<Point> {
    match points {
        [] | [_] => points.to_vec(),
        _ => {
            let mut selected_point: Option<&Point> = None;
            let mut previous = points.first().unwrap();
            let mut new_points = Vec::new();
            new_points.push(*previous);

            for point in points[1..].iter() {
                selected_point = Some(point);

                if square_distance(selected_point.unwrap(), &previous) < tolerance {
                    continue;
                }

                new_points.push(*selected_point.unwrap());
                previous = selected_point.unwrap();
            }

            if let Some(point) = selected_point {
                if point != previous {
                    new_points.push(*point);
                }
            }

            new_points
        }
    }
}

pub(crate) fn simplify_douglas_peucker(points: &[Point], tolerance: f64) -> Vec<Point> {
    match points {
        [] | [_] => points.to_vec(),
        _ => {
            let first = points.first().unwrap();
            let last = points.last().unwrap();

            let mut found_index = 0;
            let mut best_distance = 0.0;

            for (index, point) in points.iter().enumerate() {
                if point == first || point == last {
                    continue;
                }

                let distance = segment_square_distance(&point, &first, &last);
                if distance >= best_distance {
                    best_distance = distance;
                    found_index = index;
                }
            }

            if best_distance > tolerance {
                let left_part = &points[..found_index + 1];
                let right_part = &points[found_index..];

                let mut left_simplified = simplify_douglas_peucker(left_part, tolerance);
                let right_simplified = simplify_douglas_peucker(right_part, tolerance).to_vec();

                left_simplified.pop();

                left_simplified.extend(right_simplified);
                return left_simplified;
            }

            return vec![first.clone(), last.clone()];
        }
    }
}

#[cfg(test)]
mod tests {
    use crate::algorithms::{segment_square_distance, square_distance};
    use crate::point::Point;
    use serde::Deserialize;
    use std::fs;
    use crate::{simplify_douglas_peucker, simplify_radial_distance};

    #[derive(Deserialize)]
    struct Coordinates {
        x: f64,
        y: f64,
    }

    impl Coordinates {
        fn to_point(&self) -> Point {
            Point::Point2D(self.x, self.y)
        }
    }

    #[derive(Deserialize)]
    struct PointDistance {
        left: Coordinates,
        right: Coordinates,
        distance: f64,
    }

    impl PointDistance {
        fn left_point(&self) -> Point {
            Point::Point2D(self.left.x, self.left.y)
        }
        fn right_point(&self) -> Point {
            Point::Point2D(self.right.x, self.right.y)
        }
    }

    #[derive(Deserialize)]
    struct SegmentDistance {
        point: Coordinates,
        start: Coordinates,
        end: Coordinates,
        distance: f64,
    }

    impl SegmentDistance {
        fn point_point(&self) -> Point {
            Point::Point2D(self.point.x, self.point.y)
        }
        fn start_point(&self) -> Point {
            Point::Point2D(self.start.x, self.start.y)
        }
        fn end_point(&self) -> Point {
            Point::Point2D(self.end.x, self.end.y)
        }
    }

    #[derive(Deserialize)]
    struct Simplification {
        tolerance: f64,
        result: Vec<Coordinates>,
    }

    impl Simplification {
        fn points(&self) -> Vec<Point> {
            self.result.iter().map(Coordinates::to_point).collect()
        }
    }

    fn get_point_distances() -> Vec<PointDistance> {
        let contents = fs::read_to_string("../test-data/point-distances.json")
            .expect("Something went wrong reading the file");

        serde_json::from_str(&contents).unwrap()
    }

    fn get_segment_distances() -> Vec<SegmentDistance> {
        let contents = fs::read_to_string("../test-data/segment-distances.json")
            .expect("Something went wrong reading the file");

        serde_json::from_str(&contents).unwrap()
    }

    fn get_radial_simplifications() -> Vec<Simplification> {
        let contents = fs::read_to_string("../test-data/radial-simplifications.json")
            .expect("Something went wrong reading the file");

        serde_json::from_str(&contents).unwrap()
    }

    fn get_douglas_simplifications() -> Vec<Simplification> {
        let contents = fs::read_to_string("../test-data/douglas-peucker-simplifications.json")
            .expect("Something went wrong reading the file");

        serde_json::from_str(&contents).unwrap()
    }

    fn get_points() -> Vec<Point> {
        let contents = fs::read_to_string("../test-data/points.json")
            .expect("Something went wrong reading the file");

        let points: Vec<Coordinates> = serde_json::from_str(&contents).unwrap();
        points.iter().map(Coordinates::to_point).collect()
    }

    #[test]
    fn correctly_calculate_squared_distance() {
        let distances = get_point_distances();

        for case in distances {
            let left = case.left_point();
            let right = case.right_point();

            let result = square_distance(&left, &right);
            assert!((result - case.distance).abs() < 0.00001);
        }
    }

    #[test]
    fn correctly_calculate_segment_distance() {
        let distances = get_segment_distances();

        for case in distances {
            let point = case.point_point();
            let start = case.start_point();
            let end = case.end_point();

            let result = segment_square_distance(&point, &start, &end);
            assert!((result - case.distance).abs() < 0.00001);
        }
    }

    #[test]
    fn correctly_simplify_with_radial_distance() {
        let cases = get_radial_simplifications();
        let points = get_points();

        for case in cases {
            let tolerance = case.tolerance.powf(2.0);
            let result = simplify_radial_distance(&points, tolerance);
            assert_eq!(result, case.points());
        }
    }

    #[test]
    fn correctly_simplify_with_douglas_peucker() {
        let cases = get_douglas_simplifications();
        let points = get_points();

        for case in cases {
            let tolerance = case.tolerance.powf(2.0);
            let result = simplify_douglas_peucker(&points, tolerance);
            assert_eq!(result, case.points());
        }
    }
}
