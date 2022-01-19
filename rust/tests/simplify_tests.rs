use rust_simplify::{simplify, point::Point, point::Point::Point2D};
use serde::Deserialize;
use std::fs;

#[test]
fn returns_empty_list() {
    let input = Vec::<Point>::new();
    let result = simplify(&input, 1.0, false);
    assert_eq!(result.len(), 0);
}

#[test]
fn returns_list_with_one_element() {
    let mut input = Vec::new();
    input.push(Point2D(1.0, 2.0));
    let result = simplify(&input, 1.0, false);
    assert_eq!(result.len(), 1);
}

#[test]
fn returns_list_with_two_elements() {
    let mut input = Vec::new();
    input.push(Point2D(1.0, 2.0));
    input.push(Point2D(2.0, 3.0));
    let result = simplify(&input, 1.0, false);
    assert_eq!(result.len(), 2);
}

#[test]
fn correctly_simplifies_with_algorithm() {
    #[derive(Deserialize)]
    struct DataPoint {
        x: f64,
        y: f64,
    }

    impl DataPoint {
        fn to_point(&self) -> Point {
            Point2D(self.x, self.y)
        }
    }

    #[derive(Deserialize)]
    #[serde(rename_all = "camelCase")]
    struct Simplification {
        high_quality: bool,
        tolerance: f64,
        result: Vec<DataPoint>,
    }

    let alg_contents = fs::read_to_string("../test-data/algorithm-simplifications.json")
        .expect("Something went wrong reading the file");
    let point_contents = fs::read_to_string("../test-data/points.json")
        .expect("Something went wrong reading the file");

    let cases: Vec<Simplification> = serde_json::from_str(&alg_contents).unwrap();
    let data_points: Vec<DataPoint> = serde_json::from_str(&point_contents).unwrap();
    let points: Vec<Point> = data_points.iter().map(DataPoint::to_point).collect();

    for case in cases {
        let case_points: Vec<Point> = case.result.iter().map(DataPoint::to_point).collect();
        let result = simplify(&points, case.tolerance, case.high_quality);
        assert_eq!(result, case_points)
    }
}