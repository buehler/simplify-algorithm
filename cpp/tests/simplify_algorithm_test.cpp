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
