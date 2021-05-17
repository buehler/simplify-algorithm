package simplify

import "fmt"

type Point2D struct {
	X float64
	Y float64
}

type Point3D struct {
	X float64
	Y float64
	Z float64
}

type MappablePoint interface {
	MapPoint() (float64, float64, float64)
}

func (point *Point2D) MapPoint() (float64, float64, float64) {
	return point.X, point.Y, 0
}

func (point *Point3D) MapPoint() (float64, float64, float64) {
	return point.X, point.Y, point.Z
}

func (point *Point2D) String() string {
	return fmt.Sprintf("2D Point: X = %f; Y = %f", point.X, point.Y)
}

func (point *Point3D) String() string {
	return fmt.Sprintf("3D Point: X = %f; Y = %f; Z = %f", point.X, point.Y, point.Z)
}
