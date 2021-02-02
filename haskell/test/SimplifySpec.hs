import Control.Exception (evaluate)
import Impl.Simplify
import Test.Hspec
import TestData

-- import Test.QuickCheck

main :: IO ()
main = hspec $ do
  describe "Simplify" $ do
    describe "mapPoint" $ do
      it "should map a 2d point to three coordinates" $
        mapPoint (Point2d 1.3 3.7) `shouldBe` (1.3, 3.7, 0)

      it "should map a 3d point to three coordinates" $
        mapPoint (Point3d 1.3 3.7 42) `shouldBe` (1.3, 3.7, 42)

    describe "skip" $ do
      it "should remove the first element of a list" $
        skip 1 [1, 2, 3, 4] `shouldBe` [2, 3, 4]

      it "should remove the multiple elements of a list" $
        skip 3 [1, 2, 3, 4] `shouldBe` [4]

    describe "skipWhile" $ do
      it "should remove elements of a list until the predicate returns false" $
        skipWhile (< 3) [1, 2, 3, 4, 5] `shouldBe` [3, 4, 5]

      it "should remove elements of a list until the predicate returns false" $
        skipWhile (== "a") ["a", "a", "b", "c"] `shouldBe` ["b", "c"]

    describe "removeLast" $ do
      it "should remove the last element of a list" $
        removeLast [1, 2, 3, 4] `shouldBe` [1, 2, 3]

    describe "squareDistance" $ do
      params <- runIO pointDistances
      let cases =
            map
              (\(PointDistance l r d) -> it ("should calc distance between " ++ show l ++ " and " ++ show r) $ do d `shouldBe` squareDistance l r)
              params
      sequence_ cases

    describe "segmentSquareDistance" $ do
      params <- runIO segmentDistances
      let cases =
            map
              (\(SegmentDistance point start end distance) -> it ("should calc distance between " ++ show point ++ " and segment start " ++ show start ++ " end " ++ show end) $ do distance `shouldBe` segmentSquareDistance point start end)
              params
      sequence_ cases

    describe "simplifyRadialDistance" $ do
      points <- runIO points
      params <- runIO radialSimplifications
      let cases =
            map
              (\(Simplification tolerance result) -> it ("should calc correct result with tolerance " ++ show tolerance) $ do result `shouldBe` simplifyRadialDistance points (tolerance * tolerance))
              params
      sequence_ cases

    describe "simplifyDouglasPeuker" $ do
      points <- runIO points
      params <- runIO douglasPeuckerSimplifications
      let cases =
            map
              (\(Simplification tolerance result) -> it ("should calc correct result with tolerance " ++ show tolerance) $ do result `shouldBe` simplifyDouglasPeuker points (tolerance * tolerance))
              params
      sequence_ cases

    describe "simplify" $ do
      it "should return empty list" $
        simplify [] 1 True `shouldBe` []

      it "should return list with one point" $
        simplify [Point2d 1 2] 1 True `shouldBe` [Point2d 1 2]

      it "should return list with two points" $
        simplify [Point3d 1 2 3, Point3d 0 0 0] 1 True `shouldBe` [Point3d 1 2 3, Point3d 0 0 0]

      points <- runIO points
      params <- runIO algorithmSimplifications
      let cases =
            map
              (\(AlgorithmicSimplification tolerance highQuality result) -> it ("should calc correct result with tolerance " ++ show tolerance ++ " and highquality " ++ show highQuality) $ do result `shouldBe` simplify points tolerance highQuality)
              params
      sequence_ cases
