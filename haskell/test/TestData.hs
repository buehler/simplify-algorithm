{-# LANGUAGE DeriveGeneric #-}
{-# LANGUAGE DuplicateRecordFields #-}
{-# LANGUAGE OverloadedStrings #-}

module TestData
  ( PointDistance (..),
    SegmentDistance (..),
    Simplification (..),
    AlgorithmicSimplification (..),
    points,
    pointDistances,
    segmentDistances,
    radialSimplifications,
    douglasPeuckerSimplifications,
    algorithmSimplifications,
  )
where

import Control.Monad (MonadPlus (mzero))
import Data.Aeson
  ( FromJSON (parseJSON),
    Value (Object),
    eitherDecode,
    (.:),
  )
import qualified Data.ByteString.Lazy as B
import GHC.Generics (Generic)
import Impl.Simplify (Point (Point2d))

instance FromJSON Point where
  parseJSON (Object json) =
    Point2d
      <$> json .: "x"
        <*> json .: "y"

data PointDistance = PointDistance
  { left :: Point,
    right :: Point,
    distance :: Double
  }
  deriving (Generic)

data SegmentDistance = SegmentDistance
  { point :: Point,
    start :: Point,
    end :: Point,
    distance :: Double
  }
  deriving (Generic)

data Simplification = Simplification
  { tolerance :: Double,
    result :: [Point]
  }
  deriving (Generic)

data AlgorithmicSimplification = AlgorithmicSimplification
  { tolerance :: Double,
    highQuality :: Bool,
    result :: [Point]
  }
  deriving (Generic)

instance FromJSON PointDistance

instance FromJSON SegmentDistance

instance FromJSON Simplification

instance FromJSON AlgorithmicSimplification

pointsFile :: FilePath
pointsFile = "../test-data/points.json"

distanceFile :: FilePath
distanceFile = "../test-data/point-distances.json"

segmentFile :: FilePath
segmentFile = "../test-data/segment-distances.json"

radialSimplificationFile :: FilePath
radialSimplificationFile = "../test-data/radial-simplifications.json"

douglasPeuckerSimplificationFile :: FilePath
douglasPeuckerSimplificationFile = "../test-data/douglas-peucker-simplifications.json"

algorithmicSimplificationFile :: FilePath
algorithmicSimplificationFile = "../test-data/algorithm-simplifications.json"

getJSON :: FilePath -> IO B.ByteString
getJSON = B.readFile

points :: IO [Point]
points = do
  json <- (eitherDecode <$> getJSON pointsFile) :: IO (Either String [Point])
  case json of
    Left err -> error err
    Right values -> return values

pointDistances :: IO [PointDistance]
pointDistances = do
  json <- (eitherDecode <$> getJSON distanceFile) :: IO (Either String [PointDistance])
  case json of
    Left err -> error err
    Right values -> return values

segmentDistances :: IO [SegmentDistance]
segmentDistances = do
  json <- (eitherDecode <$> getJSON segmentFile) :: IO (Either String [SegmentDistance])
  case json of
    Left err -> error err
    Right values -> return values

radialSimplifications :: IO [Simplification]
radialSimplifications = do
  json <- (eitherDecode <$> getJSON radialSimplificationFile) :: IO (Either String [Simplification])
  case json of
    Left err -> error err
    Right values -> return values

douglasPeuckerSimplifications :: IO [Simplification]
douglasPeuckerSimplifications = do
  json <- (eitherDecode <$> getJSON douglasPeuckerSimplificationFile) :: IO (Either String [Simplification])
  case json of
    Left err -> error err
    Right values -> return values

algorithmSimplifications :: IO [AlgorithmicSimplification]
algorithmSimplifications = do
  json <- (eitherDecode <$> getJSON algorithmicSimplificationFile) :: IO (Either String [AlgorithmicSimplification])
  case json of
    Left err -> error err
    Right values -> return values
