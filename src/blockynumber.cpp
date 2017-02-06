#include <stdlib.h>
#include <math.h>
#include <iostream>

#include <blockynumber.hpp>

using namespace std;;

BlockyNumber::BlockyNumber(int64_t number, int16_t exponent)
{
    IsNegative = number < 0;
    this->Number = abs(number);

    if (number != 0)
        this->Exponent = exponent;
    else
        this->Exponent = 0;
    NeededBitsNumber = uint8_t(floor(log2(abs(number))) + 1);
    NeededBitsExponent = uint8_t(floor(log2(abs(exponent))) + 1);
}


BlockyNumber& BlockyNumber::add_each(BlockyNumber const& other)
{
    BlockyNumber& num = *this;
    num.Number += other.Number * (other.IsNegative ? -1 : 1);
    num.Exponent += other.Exponent;
    return num;
}

BlockyNumber& BlockyNumber::subtract_each(BlockyNumber const& other)
{
    BlockyNumber& num = *this;
    num.Number -= other.Number * (other.IsNegative ? -1 : 1);
    num.Exponent -= other.Exponent;
    return num;
}

BlockyNumber& BlockyNumber::linear_multiply_each(int32_t const& num)
{
    BlockyNumber& res = *this;
    res.Number *= num;
    res.Exponent *= int16_t(num);
    return res;
}

double BlockyNumber::reconstructed() const
{
    return Number * (IsNegative ? -1 : 1) * pow(10, Exponent);
}

void BlockyNumber::recalculate_metadata()
{
    IsNegative = Number < 0;
    Number = abs(Number);
    NeededBitsNumber = uint8_t(floor(log2(abs(Number))) + 1);
    NeededBitsExponent = uint8_t(floor(log2(abs(Exponent))) + 1);
}

bool BlockyNumber::equals(BlockyNumber const& other)
{
    return IsNegative == other.IsNegative
        && Number == other.Number
        && Exponent == other.Exponent;
}

string BlockyNumber::to_s()
{
    string s;
    s += "Reconstructed: " + to_string(reconstructed()) + ", ";
    s += "Number: "
        + to_string(Number)
        + "(" + to_string(NeededBitsNumber) + ")"
        + ", ";
    s += "Exponent: "
        + to_string(Exponent)
        + "(" + to_string(NeededBitsExponent) + ")";
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
        {
            auto valueNoComma = value;
            if (commaIndex != valueLength)
                valueNoComma.replace(commaIndex, 1, string(""));
            else
                commaIndex = 0;
            return BlockyNumber
            (
                int64_t(stoll(valueNoComma.substr(0, i))),
                int16_t(-(i - 1) + commaIndex + stoll(value.substr(i + 1)))
            );
        }
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
    BlockyNumber b(f.Number, f.Exponent);
    b.Number += (int32_t)(s.Number * pow(10, s.Exponent - f.Exponent));
    b.recalculate_metadata();
    return b;
}

BlockyNumber operator-(BlockyNumber const& f, BlockyNumber const& s)
{
    BlockyNumber b(f.Number, f.Exponent);
    b.Number -= (int32_t)(s.Number * pow(10, s.Exponent - f.Exponent));
    b.recalculate_metadata();
    return b;
}
