#include <catch2/catch_test_macros.hpp>

#include <simplify_algorithm.hpp>

TEST_CASE("Run Simplify Func", "[simplify]")
{
    simplify::simplify();
    SUCCEED("Simplify function executed successfully");
}
