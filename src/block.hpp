#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <blockymetadata.hpp>
#include <methods/methods.hpp>

class CompressionMethod;

static const int PATTERNTYPE_COUNT = 4;
enum struct PatternType { Same, Offset, Pingpong, Reserved };
static const int SAVINGGRADE_COUNT = 3;
enum struct SavingGrade { Exp, NoExp, Pattern };

struct Block
{
    int32_t Index;
    bool HasExponent;
    bool HasPattern;
    PatternType Pattern;

    int16_t Exponent;
    bool OverrideGlobalNb;
    uint8_t NeededBits;
    uint8_t Length;
    bool AbsoluteSign;
    bool IsSignNegative;
    int64_t BiggestNumber;
    bool IsValid;

    CompressionMethod* Method;

	Block()
		: Index(0)
		, HasExponent(false)
		, HasPattern(false)
		, Pattern(PatternType::Same)
		, Exponent(0)
		, OverrideGlobalNb(false)
		, NeededBits(0)
		, Length(0)
		, AbsoluteSign(false)
		, IsSignNegative(false)
		, BiggestNumber(0)
		, IsValid(false)
		, Method(NULL) { }

    Block
    (
        int32_t index,
        int16_t exponent,
        uint8_t nb,
        bool isSignNegative,
        int64_t biggestNumber,
        CompressionMethod** methods,
        Methods const method,
        bool hasPattern
    );

    bool is_valid();
    SavingGrade savinggrade() const;
    bool should_override_nb(BlockyMetadata const& metadata) const;
    int32_t difference_with_nb
    (
        BlockyMetadata const& metadata,
        uint8_t& newNb
    );

    Block& operator=(Block const& other);
};

#endif /* end of include guard: BLOCK_HPP */
