#include "catch.hpp"

#include <iostream>
#include <blockynumber.hpp>

TEST_CASE("BlockyNumber parser test", "[BlockyNumber::parse]")
{
    // testing the most happening case
    // positive number with negative e
    auto number = BlockyNumber::parse("1.9898e-10");
    REQUIRE(number.Number == 19898);
    REQUIRE(number.NeededBitsNumber == 15);
    REQUIRE(number.NeededBitsExponent == 4);
    REQUIRE(number.Exponent == -14);
    
    //positive number with positive e with + sign
    number = BlockyNumber::parse("1.234e+20");
    REQUIRE(number.Number == 1234);
    REQUIRE(number.NeededBitsNumber == 11);
    REQUIRE(number.NeededBitsExponent == 5);
    REQUIRE(number.Exponent == 17);
    
    //positive number with positive e without + sign
    number = BlockyNumber::parse("1.23e20");
    REQUIRE(number.Number == 123);
    REQUIRE(number.NeededBitsNumber == 7);
    REQUIRE(number.NeededBitsExponent == 5);
    REQUIRE(number.Exponent == 18);
    
    //negative number with positive e with + sign
    number = BlockyNumber::parse("-1.2e20");
    REQUIRE(number.Number == 12);
    REQUIRE(number.NeededBitsNumber == 4);
    REQUIRE(number.NeededBitsExponent == 5);
    REQUIRE(number.IsNegative);
    REQUIRE(number.Exponent == 19);
    
    //negative number with positive e with + sign
    number = BlockyNumber::parse("-1.111e-20");
    REQUIRE(number.Number == 1111);
    REQUIRE(number.NeededBitsNumber == 11);
    REQUIRE(number.NeededBitsExponent == 5);
    REQUIRE(number.IsNegative);
    REQUIRE(number.Exponent == -23);
    
    
    //testing more unusual cases
    number = BlockyNumber::parse("1.0e+20");
    REQUIRE(number.Number == 10);
    REQUIRE(number.Exponent == 19);
    
    number = BlockyNumber::parse("10000");
    REQUIRE(number.Number == 10000);
    REQUIRE(number.Exponent == 0);
    
    number = BlockyNumber::parse("-10000");
    REQUIRE(number.Number == 10000);
    REQUIRE(number.IsNegative);
    REQUIRE(number.Exponent == 0);
    
    number = BlockyNumber::parse("1e10");
    REQUIRE(number.Number == 1);
    REQUIRE(number.Exponent == 10);
    
    number = BlockyNumber::parse("1e-10");
    REQUIRE(number.Number == 1);
    REQUIRE(number.Exponent == -10);
}
