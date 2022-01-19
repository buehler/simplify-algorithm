use crate::algorithms::{simplify_douglas_peucker, simplify_radial_distance};
use crate::point::Point;

mod algorithms;
pub mod point;

pub fn simplify(points: &[Point], tolerance: f64, high_quality: bool) -> Vec<Point> {
    if points.len() <= 2 {
        points.to_vec()
    } else {
        let squared_tolerance = tolerance.powf(2.0);

        if high_quality {
            simplify_douglas_peucker(points, squared_tolerance)
        } else {
            simplify_douglas_peucker(
                &simplify_radial_distance(points, squared_tolerance),
                squared_tolerance,
            )
        }
    }
}
