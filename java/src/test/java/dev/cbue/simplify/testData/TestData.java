package dev.cbue.simplify.testData;

import com.fasterxml.jackson.databind.ObjectMapper;
import dev.cbue.simplify.models.Point2d;

import java.io.File;
import java.util.stream.Stream;

public class TestData {
    public static Stream<PointDistance> pointDistances() {
        var mapper = new ObjectMapper();
        try {
            var data = mapper.readValue(new File("../test-data/point-distances.json"), PointDistance[].class);
            return Stream.of(data);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return Stream.empty();
    }

    public static Stream<SegmentDistance> segmentDistances() {
        var mapper = new ObjectMapper();
        try {
            var data = mapper.readValue(new File("../test-data/segment-distances.json"), SegmentDistance[].class);
            return Stream.of(data);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return Stream.empty();
    }

    public static Point2d[] points() {
        var mapper = new ObjectMapper();
        try {
            return mapper.readValue(new File("../test-data/points.json"), Point2d[].class);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return new Point2d[0];
    }

    public static Stream<Simplification> radialSimplifications() {
        var mapper = new ObjectMapper();
        try {
            var data = mapper.readValue(new File("../test-data/radial-simplifications.json"), Simplification[].class);
            return Stream.of(data);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return Stream.empty();
    }

    public static Stream<Simplification> douglasPeuckerSimplifications() {
        var mapper = new ObjectMapper();
        try {
            var data = mapper.readValue(new File("../test-data/douglas-peucker-simplifications.json"), Simplification[].class);
            return Stream.of(data);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return Stream.empty();
    }

    public static Stream<AlgorithmicSimplification> algorithmicSimplifications() {
        var mapper = new ObjectMapper();
        try {
            var data = mapper.readValue(new File("../test-data/algorithm-simplifications.json"), AlgorithmicSimplification[].class);
            return Stream.of(data);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return Stream.empty();
    }
}
