#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <methods/compressionmethod.hpp>
#include <blockymetadata.hpp>

enum struct PatternType { Same, Offset, Pingpong, Reserved, Count };
enum struct SavingGrade { Exp, NoExp, Pattern, Count };

struct Block
{
    int32_t const Index;
    bool HasExponent;
    bool HasPattern;
    PatternType Pattern;

    int16_t Exponent;
    bool OverrideGlobalNb;
    uint8_t NeededBits;
    uint8_t Length;
    bool AbsoluteSign;
    bool IsSignNegative;
    uint64_t BiggestNumber;
    bool IsValid;

    CompressionMethod const* Method;

    Block
    (
        int32_t index,
        int16_t exponent,
        uint8_t nb,
        bool isSignNegative,
        int64_t biggestNumber,
        CompressionMethod const** methods,
        Methods const method,
        bool hasPattern
    );

    bool is_valid();
    SavingGrade savinggrade();
    bool should_override_nb(BlockyMetadata const& metadata);
    int32_t difference_with_nb
    (
        BlockyMetadata const& metadata,
        uint8_t& newNb
    );
};

#endif /* end of include guard: BLOCK_HPP */
