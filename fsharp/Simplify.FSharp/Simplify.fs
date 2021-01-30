namespace Simplify

type Point =
    | Point2d of float * float
    | Point3d of float * float * float

module Simplify =
    let private mapPoint point =
        match point with
        | Point2d (x, y) -> (x, y, 0.0)
        | Point3d (x, y, z) -> (x, y, z)

    let internal squareDistance left right =
        let squareDiff (xl, yl, zl) (xr, yr, zr) =
            ((xl - xr) ** 2.0)
            + ((yl - yr) ** 2.0)
            + ((zl - zr) ** 2.0)

        squareDiff (mapPoint left) (mapPoint right)

    let internal segmentSquareDistance point segmentStart segmentEnd =
        let sq a = a * a

        let (x, y, z) = mapPoint point
        let (startX, startY, startZ) = mapPoint segmentStart
        let (endX, endY, endZ) = mapPoint segmentEnd

        let deltaX = endX - startX
        let deltaY = endY - startY
        let deltaZ = endZ - startZ

        let result rx ry rz =
            let dx = x - rx
            let dy = y - ry
            let dz = z - rz
            sq dx + sq dy + sq dz

        if deltaX <> 0.0 || deltaY <> 0.0 || deltaZ <> 0.0 then
            let t =
                ((x - startX) * deltaX
                 + (y - startY) * deltaY
                 + (z - startZ) * deltaZ)
                / (sq deltaX + sq deltaY + sq deltaZ)

            if t > 1.0
            then result endX endY endZ
            else result (startX + deltaX * t) (startY + deltaY * t) (startZ + deltaZ * t)
        else
            result startX startY startZ

    let rec internal simplifyRadialDistance points tolerance =
        match points with
        | []
        | [ _ ] -> points
        | point :: rest ->
            point
            :: simplifyRadialDistance
                (rest
                 |> List.skipWhile (fun p -> squareDistance p point < tolerance))
                   tolerance

    let rec internal simplifyDouglasPeucker points tolerance =
        match points with
        | []
        | [ _ ]
        | [ _; _ ] -> points
        | _ ->
            let first = points.Head
            let last = points |> List.last

            let (index, distance) =
                points
                |> List.mapi (fun index point ->
                    if point = first || point = last
                    then (index, -1.0)
                    else (index, segmentSquareDistance point first last))
                |> List.maxBy snd

            if distance > tolerance then
                ((simplifyDouglasPeucker (points |> List.take (index + 1)) tolerance)
                 |> List.rev
                 |> List.skip 1
                 |> List.rev)
                @ (simplifyDouglasPeucker (points |> List.skip (index)) tolerance)
            else
                [ first; last ]

    let simplify points tolerance highQuality =
        match points with
        | []
        | [ _ ]
        | [ _; _ ] -> points
        | _ ->
            let squaredTolerance = tolerance * tolerance

            if highQuality
            then simplifyDouglasPeucker points squaredTolerance
            else simplifyDouglasPeucker (simplifyRadialDistance points squaredTolerance) squaredTolerance
