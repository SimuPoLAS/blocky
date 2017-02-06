#include "catch.hpp"

#include <blockynumber.hpp>

TEST_CASE("BlockyNumber parser test", "[BlockyNumber::parse]")
{
    auto number = BlockyNumber::parse("1.9898e-10");
    
    REQUIRE(number.Number == 19898);
    REQUIRE(number.Exponent == -10);
}
