package simplify

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"os"
)

type pointDistance struct {
	Left     Point2D
	Right    Point2D
	Distance float64
}

type segmentDistance struct {
	Point    Point2D
	Start    Point2D
	End      Point2D
	Distance float64
}

type simplification struct {
	Tolerance float64
	Result    []Point2D
}

type algorithmicSimplification struct {
	HighQuality bool
	Tolerance   float64
	Result      []Point2D
}

func mapPoints(points *[]Point2D) *[]MappablePoint {
	result := make([]MappablePoint, len(*points))

	for x := 0; x < len(*points); x++ {
		var value MappablePoint = &(*points)[x]
		result[x] = value
	}

	return &result
}

func points() *[]MappablePoint {
	jsonFile, err := os.Open("../../../test-data/points.json")
	if err != nil {
		fmt.Println(err)
	}

	defer jsonFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonFile)

	var objects []Point2D
	json.Unmarshal(byteValue, &objects)

	return mapPoints(&objects)
}

func pointDistances() *[]pointDistance {
	jsonFile, err := os.Open("../../../test-data/point-distances.json")
	if err != nil {
		fmt.Println(err)
	}

	defer jsonFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonFile)

	var objects []pointDistance
	json.Unmarshal(byteValue, &objects)

	return &objects
}

func segmentDistances() *[]segmentDistance {
	jsonFile, err := os.Open("../../../test-data/segment-distances.json")
	if err != nil {
		fmt.Println(err)
	}

	defer jsonFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonFile)

	var objects []segmentDistance
	json.Unmarshal(byteValue, &objects)

	return &objects
}

func radialSimplifications() *[]simplification {
	jsonFile, err := os.Open("../../../test-data/radial-simplifications.json")
	if err != nil {
		fmt.Println(err)
	}

	defer jsonFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonFile)

	var objects []simplification
	json.Unmarshal(byteValue, &objects)

	return &objects
}

func douglasPeuckerSimplifications() *[]simplification {
	jsonFile, err := os.Open("../../../test-data/douglas-peucker-simplifications.json")
	if err != nil {
		fmt.Println(err)
	}

	defer jsonFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonFile)

	var objects []simplification
	json.Unmarshal(byteValue, &objects)

	return &(objects)
}

func algorithmicSimplifications() *[]algorithmicSimplification {
	jsonFile, err := os.Open("../../../test-data/algorithm-simplifications.json")
	if err != nil {
		fmt.Println(err)
	}

	defer jsonFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonFile)

	var objects []algorithmicSimplification
	json.Unmarshal(byteValue, &objects)

	return &objects
}
