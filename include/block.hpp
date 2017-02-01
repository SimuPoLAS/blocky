#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <methods/compressionmethod.hpp>

enum struct PatternType { Same, Offset, Pingpong, Reserved };
enum struct SavingGrade { Exp, NoExp, Pattern };

struct Block
{
    static const Block INVALID_BLOCK = default(block);

    int32_t const index;
    bool hasExponent;
    bool hasPattern;
    PatternType pattern;

    int16_t exponent;
    bool overrideGlobalNb;
    uint8_t neededBits;
    uint8_t length;
    bool absoluteSign;
    bool isSignNegative;
    uint64_t biggestNumber;

    CompressionMethod method;

    Block
    (
        int32_t index,
        int16_t exponent,
        uint8_t nb,
        bool isSignNegative,
        int64_t biggestNumber,
        Blockfinding context,
        Blockfinding.Methods method,
        bool hasPattern
    );
};

#endif /* end of include guard: BLOCK_HPP */
