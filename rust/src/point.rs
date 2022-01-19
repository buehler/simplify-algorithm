#[derive(Debug, Copy, Clone, PartialEq)]
pub enum Point {
    Point2D(f64, f64),
    Point3D(f64, f64, f64),
}

impl Point {
    pub(crate) fn map(&self) -> (f64, f64, f64) {
        match self {
            Point::Point2D(x, y) => (*x, *y, 0.0),
            Point::Point3D(x, y, z) => (*x, *y, *z),
        }
    }
}

#[cfg(test)]
mod tests {
    use crate::point::Point::{Point2D, Point3D};

    #[test]
    fn map_2d_point() {
        let point = Point2D(1.0, 2.0);
        assert_eq!((1.0, 2.0, 0.0), point.map())
    }

    #[test]
    fn map_3d_point() {
        let point = Point3D(1.0, 2.0, 3.0);
        assert_eq!((1.0, 2.0, 3.0), point.map())
    }
}
