module Impl.Simplify where

data Point
  = Point2d Double Double
  | Point3d Double Double Double
  deriving (Show, Eq)

mapPoint :: Point -> (Double, Double, Double)
mapPoint (Point2d x y) = (x, y, 0)
mapPoint (Point3d x y z) = (x, y, z)

-- skipWhile :: (a -> Bool) -> [a] -> [a]
-- skipWhile _ [] = []
-- skipWhile predicate (x : xs) =
--   if predicate x
--     then skipWhile predicate xs
--     else x : xs

-- -- todo optimize with recursive call (is better than comprehension)
-- -- skip number list = [snd x | x <- zip [0 ..] list, fst x >= number]
-- skip :: (Num a1, Enum a1, Ord a1) => a1 -> [a2] -> [a2]
-- skip _ [] = []
-- skip number list =
--   let skipCount number list@((index, obj) : xs) =
--         if index < number
--           then skipCount number xs
--           else list
--    in map snd (skipCount number (zip [0 ..] list))

removeLast = reverse . drop 1 . reverse

squareDistance :: Point -> Point -> Double
squareDistance left right =
  let squareDiff (xl, yl, zl) (xr, yr, zr) =
        ((xl - xr) ** 2)
          + ((yl - yr) ** 2)
          + ((zl - zr) ** 2)
   in squareDiff (mapPoint left) (mapPoint right)

segmentSquareDistance :: Point -> Point -> Point -> Double
segmentSquareDistance point start end =
  let sq a = a * a
      (x, y, z) = mapPoint point
      (startX, startY, startZ) = mapPoint start
      (endX, endY, endZ) = mapPoint end
      deltaX = endX - startX
      deltaY = endY - startY
      deltaZ = endZ - startZ
      result rx ry rz =
        let dx = x - rx
            dy = y - ry
            dz = z - rz
         in sq dx + sq dy + sq dz
   in if deltaX /= 0 || deltaX /= 0 || deltaZ /= 0
        then
          let t =
                ( (x - startX) * deltaX
                    + (y - startY) * deltaY
                    + (z - startZ) * deltaZ
                )
                  / (sq deltaX + sq deltaY + sq deltaZ)
           in if t > 1
                then result endX endY endZ
                else result (startX + deltaX * t) (startY + deltaY * t) (startZ + deltaZ * t)
        else result startX startY startZ

simplifyRadialDistance :: [Point] -> Double -> [Point]
simplifyRadialDistance [] _ = []
simplifyRadialDistance [point] _ = [point]
simplifyRadialDistance (point : points) tolerance =
  point : simplifyRadialDistance (dropWhile (\p -> squareDistance p point < tolerance) points) tolerance

simplifyDouglasPeuker :: [Point] -> Double -> [Point]
simplifyDouglasPeuker [] _ = []
simplifyDouglasPeuker points@[_] _ = points
simplifyDouglasPeuker points@[_, _] _ = points
simplifyDouglasPeuker points tolerance =
  let (first : _) = points
      (last : _) = reverse points
      distances = zipWith (\i p -> (if p == first || p == last then (i, -1) else (i, segmentSquareDistance p first last))) [0 ..] points
      (index, distance) = foldr1 (\left@(_, distanceLeft) right@(_, distanceRight) -> if distanceLeft >= distanceRight then left else right) distances
   in if distance > tolerance
        then removeLast (simplifyDouglasPeuker (take (index + 1) points) tolerance) ++ simplifyDouglasPeuker (drop index points) tolerance
        else [first, last]

simplify :: [Point] -> Double -> Bool -> [Point]
simplify [] _ _ = []
simplify points@[_] _ _ = points
simplify points@[_, _] _ _ = points
simplify points tolerance highQuality =
  let squaredTolerance = tolerance * tolerance
   in if highQuality
        then simplifyDouglasPeuker points squaredTolerance
        else simplifyDouglasPeuker (simplifyRadialDistance points squaredTolerance) squaredTolerance
