#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

#include <simplify_algorithm.hpp>
#include <points.hpp>

#include <algorithms.hpp>

using json = nlohmann::json;

std::vector<simplify::IPoint *> getPoints()
{
    std::ifstream f("points.json");
    auto data = json::parse(f);
    std::vector<simplify::IPoint *> v;
    for (auto &el : data)
    {
        auto p = simplify::IPoint::create(el["x"], el["y"]);
        v.push_back(p);
    }
    return v;
}

struct DistanceTestCase
{
    simplify::IPoint *left;
    simplify::IPoint *right;
    double distance;
};

void from_json(const json &j, DistanceTestCase &testCase)
{
    testCase.left = simplify::IPoint::create(j["left"]["x"], j["left"]["y"]);
    testCase.right = simplify::IPoint::create(j["right"]["x"], j["right"]["y"]);
    testCase.distance = j["distance"];
}

std::vector<DistanceTestCase> getPointDistances()
{
    std::ifstream f("point-distances.json");
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open point-distances.json");
    }
    auto data = json::parse(f);
    auto parsed = data.get<std::vector<DistanceTestCase>>();
    return parsed;
}

struct SegmentDistanceTestCase
{
    simplify::IPoint *point;
    simplify::IPoint *start;
    simplify::IPoint *end;
    double distance;
};

void from_json(const json &j, SegmentDistanceTestCase &testCase)
{
    testCase.point = simplify::IPoint::create(j["point"]["x"], j["point"]["y"]);
    testCase.start = simplify::IPoint::create(j["start"]["x"], j["start"]["y"]);
    testCase.end = simplify::IPoint::create(j["end"]["x"], j["end"]["y"]);
    testCase.distance = j["distance"];
}

std::vector<SegmentDistanceTestCase> getSegmentDistances()
{
    std::ifstream f("segment-distances.json");
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open segment-distances.json");
    }
    auto data = json::parse(f);
    auto parsed = data.get<std::vector<SegmentDistanceTestCase>>();
    return parsed;
}

struct RadialSimplificationTestCase
{
    double tolerance;
    std::vector<simplify::IPoint *> result;
};

void from_json(const json &j, RadialSimplificationTestCase &testCase)
{
    testCase.tolerance = j["tolerance"];
    for (const auto &point : j["result"])
    {
        testCase.result.push_back(simplify::IPoint::create(point["x"], point["y"]));
    }
}

std::vector<RadialSimplificationTestCase> getRadialSimplificationCases()
{
    std::ifstream f("radial-simplifications.json");
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open radial-simplification.json");
    }
    auto data = json::parse(f);
    return data.get<std::vector<RadialSimplificationTestCase>>();
}

struct DouglasPeuckerTestCase
{
    double tolerance;
    std::vector<simplify::IPoint *> result;
};

void from_json(const json &j, DouglasPeuckerTestCase &testCase)
{
    testCase.tolerance = j["tolerance"];
    for (const auto &point : j["result"])
    {
        testCase.result.push_back(simplify::IPoint::create(point["x"], point["y"]));
    }
}

std::vector<DouglasPeuckerTestCase> getDouglasPeuckerCases()
{
    std::ifstream f("douglas-peucker-simplifications.json");
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open douglas-peucker-simplifications.json");
    }
    auto data = json::parse(f);
    return data.get<std::vector<DouglasPeuckerTestCase>>();
}

struct AlgorithmTestCase
{
    bool highQuality;
    double tolerance;
    std::vector<simplify::IPoint *> result;
};

void from_json(const json &j, AlgorithmTestCase &testCase)
{
    testCase.highQuality = j["highQuality"];
    testCase.tolerance = j["tolerance"];
    for (const auto &point : j["result"])
    {
        testCase.result.push_back(simplify::IPoint::create(point["x"], point["y"]));
    }
}

std::vector<AlgorithmTestCase> getAlgorithmCases()
{
    std::ifstream f("algorithm-simplifications.json");
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open algorithm-simplifications.json");
    }
    auto data = json::parse(f);
    return data.get<std::vector<AlgorithmTestCase>>();
}

SCENARIO("Simple Points can be created")
{
    GIVEN("A 2D point")
    {
        auto point2D = simplify::IPoint::create(1.0f, 2.0f);

        THEN("it should have the correct coordinates")
        {
            REQUIRE(point2D->getX() == 1.0f);
            REQUIRE(point2D->getY() == 2.0f);
            REQUIRE(point2D->getZ() == 0.0f);
        }

        THEN("it should return the correct coordinates object")
        {
            auto coords = point2D->getCoordinates();
            REQUIRE(coords.x == 1.0f);
            REQUIRE(coords.y == 2.0f);
            REQUIRE(coords.z == 0.0f);
        }
    }

    GIVEN("A 3D point")
    {
        auto point3D = simplify::IPoint::create(1.0f, 2.0f, 3.0f);

        THEN("it should have the correct coordinates")
        {
            REQUIRE(point3D->getX() == 1.0f);
            REQUIRE(point3D->getY() == 2.0f);
            REQUIRE(point3D->getZ() == 3.0f);
        }

        THEN("it should return the correct coordinates object")
        {
            auto coords = point3D->getCoordinates();
            REQUIRE(coords.x == 1.0f);
            REQUIRE(coords.y == 2.0f);
            REQUIRE(coords.z == 3.0f);
        }
    }
}

SCENARIO("Square distance calculation")
{
    GIVEN("A list of point distance test cases")
    {
        auto cases = getPointDistances();

        THEN("The list should contain 100 distance test cases")
        {
            REQUIRE(cases.size() == 100);
        }

        WHEN("Calculating square distances")
        {
            for (const auto &testCase : cases)
            {
                auto left = testCase.left;
                auto right = testCase.right;
                auto expectedDistance = testCase.distance;

                THEN("The square distance should be calculated correctly")
                {
                    auto distance = simplify::algorithms::squareDistance(*left, *right);
                    REQUIRE_THAT(
                        distance,
                        Catch::Matchers::WithinAbs(expectedDistance, 1e-6));
                }
            }
        }
    }
}

SCENARIO("Segment distance calculation")
{
    GIVEN("A list of segment distance test cases")
    {
        auto cases = getSegmentDistances();

        THEN("The list should contain 98 distance test cases")
        {
            REQUIRE(cases.size() == 98);
        }

        WHEN("Calculating segment distances")
        {
            for (const auto &testCase : cases)
            {
                auto point = testCase.point;
                auto start = testCase.start;
                auto end = testCase.end;
                auto expectedDistance = testCase.distance;

                THEN("The segment distance should be calculated correctly")
                {
                    auto distance = simplify::algorithms::segmentSquareDistance(*point, *start, *end);
                    REQUIRE_THAT(
                        distance,
                        Catch::Matchers::WithinAbs(expectedDistance, 1e-6));
                }
            }
        }
    }
}

SCENARIO("Radial simplification algorithm")
{
    GIVEN("A list of radial simplification test cases")
    {
        auto cases = getRadialSimplificationCases();

        THEN("The list should contain 4 radial simplification test cases")
        {
            REQUIRE(cases.size() == 4);
        }

        WHEN("Applying the radial simplification algorithm")
        {
            for (const auto &testCase : cases)
            {
                auto tolerance = testCase.tolerance * testCase.tolerance;
                auto expectedResult = testCase.result;
                auto points = getPoints();

                THEN("The radial simplification should produce the expected result")
                {
                    auto simplified = simplify::algorithms::simplifyRadialDistance(points, tolerance);
                    REQUIRE(simplified.size() == expectedResult.size());

                    for (size_t i = 0; i < simplified.size(); ++i)
                    {
                        auto expectedPoint = expectedResult[i];
                        auto simplifiedPoint = simplified[i];

                        REQUIRE(*expectedPoint == *simplifiedPoint);
                    }
                }
            }
        }
    }
}

SCENARIO("Douglas Peucker simplification algorithm")
{
    GIVEN("A list of douglas peucker simplification test cases")
    {
        auto cases = getDouglasPeuckerCases();

        THEN("The list should contain 4 douglas peucker simplification test cases")
        {
            REQUIRE(cases.size() == 4);
        }

        WHEN("Applying the douglas peucker simplification algorithm")
        {
            for (const auto &testCase : cases)
            {
                auto tolerance = testCase.tolerance * testCase.tolerance;
                auto expectedResult = testCase.result;
                auto points = getPoints();

                THEN("The douglas peucker simplification should produce the expected result")
                {
                    auto simplified = simplify::algorithms::simplifyDouglasPeucker(points, tolerance);
                    REQUIRE(simplified.size() == expectedResult.size());

                    for (size_t i = 0; i < simplified.size(); ++i)
                    {
                        auto expectedPoint = expectedResult[i];
                        auto simplifiedPoint = simplified[i];

                        REQUIRE(*expectedPoint == *simplifiedPoint);
                    }
                }
            }
        }
    }
}

SCENARIO("Algorithm simplification algorithm")
{
    GIVEN("A list of algorithm simplification test cases")
    {
        auto cases = getAlgorithmCases();

        THEN("The list should contain 8 algorithm simplification test cases")
        {
            REQUIRE(cases.size() == 8);
        }

        WHEN("Applying the algorithm simplification algorithm")
        {
            for (const auto &testCase : cases)
            {
                auto tolerance = testCase.tolerance;
                auto highQuality = testCase.highQuality;
                auto expectedResult = testCase.result;
                auto points = getPoints();

                THEN("The algorithm simplification should produce the expected result")
                {
                    auto simplified = simplify::simplify(points, tolerance, highQuality);
                    REQUIRE(simplified.size() == expectedResult.size());

                    for (size_t i = 0; i < simplified.size(); ++i)
                    {
                        auto expectedPoint = expectedResult[i];
                        auto simplifiedPoint = simplified[i];

                        REQUIRE(*expectedPoint == *simplifiedPoint);
                    }
                }
            }
        }
    }
}
