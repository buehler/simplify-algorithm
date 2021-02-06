import org.scalacheck.util.SerializableCanBuildFroms.listFactory
import org.scalatest.flatspec.AnyFlatSpec
import org.scalatest.matchers.should
import org.scalatest.prop.{TableDrivenPropertyChecks, TableFor3}
import org.scalatest.{Inside, Inspectors, OptionValues}
import upickle.default

case class Run(highQuality: Boolean, tolerance: Double, result: List[Point2d])

class SimplifyTest
  extends AnyFlatSpec
    with should.Matchers
    with OptionValues
    with Inside
    with Inspectors
    with TableDrivenPropertyChecks {

  implicit val runRW: default.ReadWriter[Run] = upickle.default.macroRW[Run]
  implicit val pointRW: default.ReadWriter[Point2d] = upickle.default.macroRW[Point2d]

  def loadPoints: List[Point2d] = {
    val path = os.pwd / os.up / "test-data" / "points.json"
    val json = os.read(path)
    upickle.default.read[List[Point2d]](json)
  }

  def loadData: List[Run] = {
    val path = os.pwd / os.up / "test-data" / "algorithm-simplifications.json"
    val json = os.read(path)
    upickle.default.read[List[Run]](json)
  }

  val combos: TableFor3[Boolean, Double, List[Point2d]] = Table(("highQuality", "tolerance", "result")) ++ loadData.map(run => (run.highQuality, run.tolerance, run.result))

  "The Simplify Algorithm" should "calculate the correct result" in {
    forAll(combos) { (highQuality, tolerance, result) =>
      assert(result == Simplify(loadPoints, tolerance, highQuality))
    }
  }
}
