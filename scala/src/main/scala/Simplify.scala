import scala.math.pow

abstract class Point

case class Point2d(x: Double, y: Double) extends Point

case class Point3d(x: Double, y: Double, z: Double) extends Point

object Simplify {
  private type Coordinates = (Double, Double, Double)

  private implicit def fromPoint(p: Point): Coordinates = p match {
    case p: Point2d => fromPoint2d(p)
    case p: Point3d => fromPoint3d(p)
  }

  private implicit def fromPoint2d(p: Point2d): Coordinates = (p.x, p.y, 0)

  private implicit def fromPoint3d(p: Point3d): Coordinates = (p.x, p.y, p.z)

  private def removeLast[A](list: List[A]) = list
    .reverse
    .drop(1)
    .reverse

  private def squareDistance(left: Point, right: Point) = {
    def squareDiff(l: Coordinates, r: Coordinates) = {
      val (xl, yl, zl) = l
      val (xr, yr, zr) = r
      pow(xl - xr, 2) + pow(yl - yr, 2) + pow(zl - zr, 2)
    }

    squareDiff(left, right)
  }

  private def segmentSquareDistance(point: Point, segmentStart: Point, segmentEnd: Point) = {
    def sq(a: Double) = a * a

    val (x, y, z): Coordinates = point
    val (startX, startY, startZ): Coordinates = segmentStart
    val (endX, endY, endZ): Coordinates = segmentEnd

    val (deltaX, deltaY, deltaZ) = (endX - startX, endY - startY, endZ - startZ)

    def result(rx: Double, ry: Double, rz: Double) = {
      val dx = x - rx
      val dy = y - ry
      val dz = z - rz
      sq(dx) + sq(dy) + sq(dz)
    }

    if (deltaX != 0 || deltaY != 0 || deltaZ != 0) {
      val t = ((x - startX) * deltaX
        + (y - startY) * deltaY
        + (z - startZ) * deltaZ) / (sq(deltaX) + sq(deltaY) + sq(deltaZ))
      if (t > 1)
        result(endX, endY, endZ)
      else
        result(startX + deltaX * t, startY + deltaY * t, startZ + deltaZ * t)
    } else
      result(startX, startY, startZ)
  }

  private def simplifyRadialDistance(points: List[Point], tolerance: Double): List[Point] = points match {
    case List() | List(_) => points
    case point :: rest => point :: simplifyRadialDistance(rest.dropWhile(p => squareDistance(p, point) < tolerance), tolerance)
  }

  private def simplifyDouglasPeucker(points: List[Point], tolerance: Double): List[Point] = points match {
    case List() | List(_) | List(_, _) => points
    case _ =>
      val first = points.head
      val last = points.last

      val (index, distance) = points
        .zipWithIndex
        .map { case (point, index) => if (point == first || point == last) (index, -1.0) else (index, segmentSquareDistance(point, first, last)) }
        .maxBy(_._2)

      if (distance > tolerance)
        removeLast(simplifyDouglasPeucker(points.take(index + 1), tolerance)) ++ simplifyDouglasPeucker(points.drop(index), tolerance)
      else
        List(first, last)
  }

  def apply(points: List[Point], tolerance: Double, highQuality: Boolean): List[Point] = points match {
    case List() | List(_) | List(_, _) => points
    case _ =>
      val squaredTolerance = tolerance * tolerance
      if (highQuality)
        simplifyDouglasPeucker(points, squaredTolerance)
      else
        simplifyDouglasPeucker(simplifyRadialDistance(points, squaredTolerance), squaredTolerance)
  }
}
