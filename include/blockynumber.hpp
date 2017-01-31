#ifndef BLOCKYNUMBER_HPP
#define BLOCKYNUMBER_HPP

#include <string>

using namespace std;

struct BlockyNumber
{
    bool isNegative;
    int64_t number;
    int16_t exponent;
    int8_t neededBitsNumber;
    int8_t neededBitsExponent;

    BlockyNumber(int64_t number, int16_t exponent);

    BlockyNumber& add_each(BlockyNumber const& other);
    BlockyNumber& subtract_each(BlockyNumber const& other);
    BlockyNumber& linear_multiply_each(int32_t const& num);

    double reconstructed() const;
    void recalculate_metadata();
    bool equals(BlockyNumber const& other);
    string to_s();

    operator double();
    explicit operator int();

    // bool operator<(BlockyNumber first, BlockyNumber second);
    // bool operator>(BlockyNumber first, BlockyNumber second);
    // bool operator<(BlockyNumber first, double second);
    // bool operator>(BlockyNumber first, double second);
    // bool operator<(double first, BlockyNumber second);
    // bool operator>(double first, BlockyNumber second);

    // BlockyNumber operator+(BlockyNumber first, BlockyNumber second);
    // BlockyNumber operator-(BlockyNumber first, BlockyNumber second);

    static BlockyNumber parse(string const& value);
};

#endif /* end of include guard: NUMBER_HPP */
