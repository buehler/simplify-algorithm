module TestData

open System.IO
open Microsoft.FSharp.Reflection
open FSharp.Data
open Simplify

type Distance = Point * Point * float
type SegmentDistance = Point * Point * Point * float
type RadialSimplification = float * Point list
type DouglasPeuckerSimplification = float * Point list
type SimplifySimplification = bool * float * Point list

type private JsonPoints = JsonProvider<"../../test-data/points.json">
type private JsonPointDistances = JsonProvider<"../../test-data/point-distances.json">
type private JsonSegmentDistances = JsonProvider<"../../test-data/segment-distances.json">
type private JsonRadialSimplifications = JsonProvider<"../../test-data/radial-simplifications.json">
type private JsonDouglasPeuckerSimplifications = JsonProvider<"../../test-data/douglas-peucker-simplifications.json">
type private JsonSimplifySimplifications = JsonProvider<"../../test-data/algorithm-simplifications.json">

type TestData() =

    static member points: Point list =
        async {
            let! json = JsonPoints.AsyncLoad(Path.Join("Data", "points.json"))

            return
                json
                |> Seq.map (fun p -> Point2d(float p.X, float p.Y))
                |> List.ofSeq
        }
        |> Async.RunSynchronously

    static member distances: obj [] seq =
        async {
            let! json = JsonPointDistances.AsyncLoad(Path.Join("Data", "point-distances.json"))

            return
                json
                |> Seq.map (fun p ->
                    Distance
                        (Point2d(float p.Left.X, float p.Left.Y),
                         Point2d(float p.Right.X, float p.Right.Y),
                         float p.Distance))
        }
        |> Async.RunSynchronously
        |> Seq.map FSharpValue.GetTupleFields

    static member segmentDistances: obj [] seq =
        async {
            let! json = JsonSegmentDistances.AsyncLoad(Path.Join("Data", "segment-distances.json"))

            return
                json
                |> Seq.map (fun p ->
                    SegmentDistance
                        (Point2d(float p.Point.X, float p.Point.Y),
                         Point2d(float p.Start.X, float p.Start.Y),
                         Point2d(float p.End.X, float p.End.Y),
                         float p.Distance))
        }
        |> Async.RunSynchronously
        |> Seq.map FSharpValue.GetTupleFields

    static member radialSimplifications: obj [] seq =
        async {
            let! json = JsonRadialSimplifications.AsyncLoad(Path.Join("Data", "radial-simplifications.json"))

            return
                json
                |> Seq.map (fun p ->
                    RadialSimplification
                        (float p.Tolerance,
                         p.Result
                         |> Array.map (fun r -> Point2d(float r.X, float r.Y))
                         |> List.ofArray))
        }
        |> Async.RunSynchronously
        |> Seq.map FSharpValue.GetTupleFields

    static member douglasPeuckerSimplifications: obj [] seq =
        async {
            let! json =
                JsonDouglasPeuckerSimplifications.AsyncLoad(Path.Join("Data", "douglas-peucker-simplifications.json"))

            return
                json
                |> Seq.map (fun p ->
                    DouglasPeuckerSimplification
                        (float p.Tolerance,
                         p.Result
                         |> Array.map (fun r -> Point2d(float r.X, float r.Y))
                         |> List.ofArray))
        }
        |> Async.RunSynchronously
        |> Seq.map FSharpValue.GetTupleFields

    static member simplifySimplifications: obj [] seq =
        async {
            let! json = JsonSimplifySimplifications.AsyncLoad(Path.Join("Data", "algorithm-simplifications.json"))

            return
                json
                |> Seq.map (fun p ->
                    SimplifySimplification
                        (p.HighQuality,
                         float p.Tolerance,
                         p.Result
                         |> Array.map (fun r -> Point2d(float r.X, float r.Y))
                         |> List.ofArray))
        }
        |> Async.RunSynchronously
        |> Seq.map FSharpValue.GetTupleFields
