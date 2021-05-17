package simplify

import "math"

func squareDistance(left *MappablePoint, right *MappablePoint) float64 {
	leftX, leftY, leftZ := (*left).MapPoint()
	rightX, rightY, rightZ := (*right).MapPoint()

	return math.Pow(leftX-rightX, 2) + math.Pow(leftY-rightY, 2) + math.Pow(leftZ-rightZ, 2)
}

func segmentSquareDistance(point *MappablePoint, start *MappablePoint, end *MappablePoint) float64 {
	x, y, z := (*point).MapPoint()
	startX, startY, startZ := (*start).MapPoint()
	endX, endY, endZ := (*end).MapPoint()

	deltaX := endX - startX
	deltaY := endY - startY
	deltaZ := endZ - startZ

	result := func(rx float64, ry float64, rz float64) float64 {
		dx := x - rx
		dy := y - ry
		dz := z - rz
		return dx*dx + dy*dy + dz*dz
	}

	if deltaX == 0 && deltaY == 0 && deltaZ == 0 {
		return result(startX, startY, startZ)
	}

	t := ((x-startX)*deltaX + (y-startY)*deltaY + (z-startZ)*deltaZ) / (deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ)

	if t > 1 {
		return result(endX, endY, endZ)
	}

	return result(startX+deltaX*t, startY+deltaY*t, startZ+deltaZ*t)
}

func simplifyRadialDistance(points []MappablePoint, tolerance float64) []MappablePoint {
	if len(points) <= 2 {
		return points
	}

	previous := points[0]
	var newPoints = []MappablePoint{previous}
	var selectedPoint MappablePoint

	for _, point := range points[1:] {
		selectedPoint = point

		if squareDistance(&selectedPoint, &previous) < tolerance {
			continue
		}

		newPoints = append(newPoints, selectedPoint)
		previous = selectedPoint
	}

	if selectedPoint != nil && previous != selectedPoint {
		newPoints = append(newPoints, selectedPoint)
	}

	return newPoints
}

func simplifyDouglasPeucker(points []MappablePoint, tolerance float64) []MappablePoint {
	if len(points) <= 2 {
		return points
	}

	first := points[0]
	last := points[len(points)-1]

	var foundIndex int
	var bestDistance float64
	for index, point := range points {
		if point == first || point == last {
			continue
		}

		distance := segmentSquareDistance(&point, &first, &last)
		if distance >= bestDistance {
			bestDistance = distance
			foundIndex = index
		}
	}

	if bestDistance > tolerance {
		leftPart := points[:foundIndex+1]
		rightPart := points[foundIndex:]

		left := simplifyDouglasPeucker(leftPart, tolerance)
		leftR := left[:len(left)-1]

		right := simplifyDouglasPeucker(rightPart, tolerance)

		result := append(leftR, right...)
		return result
	}

	return []MappablePoint{first, last}
}

func Simplify(points []MappablePoint, tolerance float64, highQuality bool) []MappablePoint {
	if len(points) <= 2 {
		return points
	}

	squaredTolerance := tolerance * tolerance

	if highQuality {
		return simplifyDouglasPeucker(points, squaredTolerance)
	}

	return simplifyDouglasPeucker(simplifyRadialDistance(points, squaredTolerance), squaredTolerance)
}
