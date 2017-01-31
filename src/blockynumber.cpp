#include <stdlib.h>
#include <math.h>

#include <blockynumber.hpp>

using namespace std;;

BlockyNumber::BlockyNumber(int64_t number, int16_t exponent)
{
    isNegative = number < 0;
    this->number = abs(number);

    if (number != 0)
        this->exponent = exponent;
    else
        this->exponent = 0;
    neededBitsNumber = uint8_t(floor(log2(abs(number))) + 1);
    neededBitsExponent = uint8_t(floor(log2(abs(exponent))) + 1);
}


BlockyNumber& BlockyNumber::add_each(BlockyNumber const& other)
{
    BlockyNumber& num = *this;
    num.number += other.number * (other.isNegative ? -1 : 1);
    num.exponent += other.exponent;
    return num;
}

BlockyNumber& BlockyNumber::subtract_each(BlockyNumber const& other)
{
    BlockyNumber& num = *this;
    num.number -= other.number * (other.isNegative ? -1 : 1);
    num.exponent -= other.exponent;
    return num;
}

BlockyNumber& BlockyNumber::linear_multiply_each(int32_t const& num)
{
    BlockyNumber& res = *this;
    res.number *= num;
    res.exponent *= int16_t(num);
    return res;
}

double BlockyNumber::reconstructed() const
{
    return number * (isNegative ? -1 : 1) * pow(10, exponent);
}

void BlockyNumber::recalculate_metadata()
{
    isNegative = number < 0;
    number = abs(number);
    neededBitsNumber = uint8_t(floor(log2(abs(number))) + 1);
    neededBitsExponent = uint8_t(floor(log2(abs(exponent))) + 1);
}

bool BlockyNumber::equals(BlockyNumber const& other)
{
    return isNegative == other.isNegative
        && number == other.number
        && exponent == other.exponent;
}

string BlockyNumber::to_s()
{
    string s;
    s += "Reconstructed: " + to_string(reconstructed()) + ", ";
    s += "Number: "
        + to_string(number)
        + "(" + to_string(neededBitsNumber) + ")"
        + ", ";
    s += "Exponent: "
        + to_string(exponent)
        + "(" + to_string(neededBitsExponent) + ")";
    return s;
}

BlockyNumber::operator double()
{
    return reconstructed();
}

BlockyNumber::operator int()
{
    return int(reconstructed());
}

BlockyNumber BlockyNumber::parse(string const& value)
{
    auto valueLength = uint16_t(value.length());
    auto commaIndex = valueLength;
    for (size_t i = 0; i < valueLength; i++)
    {
        if (value[i] == '.')
        {
            commaIndex = i;
            continue;
        }
        if (value[i] == 'e' || value[i] == 'E')
            return BlockyNumber
            (
                int64_t(stoll(value.substr(0, i))),
                int16_t(-(i - 1) + commaIndex + stoll(value.substr(i + 1)))
            );
    }

    return BlockyNumber
    (
        int64_t(stoll(value)),
        int16_t(commaIndex == valueLength ? 0 : -(valueLength - 1) + commaIndex)
    );
}

bool operator<(BlockyNumber const& f, BlockyNumber const& s)
{
    return f.reconstructed() < s.reconstructed();
}

bool operator>(BlockyNumber const& f, BlockyNumber const& s)
{
    return f.reconstructed() > s.reconstructed();
}

bool operator<(BlockyNumber const& f, double const& s)
{
    return f.reconstructed() < s;
}

bool operator>(BlockyNumber const& f, double const& s)
{
    return f.reconstructed() > s;
}

bool operator<(double const& f, BlockyNumber const& s)
{
    return f < s.reconstructed();
}

bool operator>(double const& f, BlockyNumber const& s)
{
    return f > s.reconstructed();
}

BlockyNumber operator+(BlockyNumber const& f, BlockyNumber const& s)
{
    BlockyNumber b(f.number, f.exponent);
    b.number += (int32_t)(s.number * pow(10, s.exponent - f.exponent));
    b.recalculate_metadata();
    return b;
}

BlockyNumber operator-(BlockyNumber const& f, BlockyNumber const& s)
{
    BlockyNumber b(f.number, f.exponent);
    b.number -= (int32_t)(s.number * pow(10, s.exponent - f.exponent));
    b.recalculate_metadata();
    return b;
}
