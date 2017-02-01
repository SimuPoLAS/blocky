#ifndef HEADERSIZES_HPP
#define HEADERSIZES_HPP

#include <blockymetadata.hpp>

struct HeaderSizes
{
    static int32_t const GlobalExponentNb = 4;
    static int32_t const GlobalNumberNb = 6;
    static int32_t const GlobalIsAbsolute = 1;
    static int32_t const Sign = 1;
    static int32_t const IsBlock = 1;
    static int32_t const BlockHasExponent = 1;
    static int32_t const BlockHasPattern = 1;
    static int32_t const BlockPatternType = 2;
    static int32_t const BlockOverrideGlobalNb = 1;
    static int32_t const BlockLength = 8;

    // Global headers
    int32_t const GlobalIsAbsoluteNegative;
    int32_t const BlockIsAbsolute;
    int32_t const BlockIsAbsoluteNegative;
    int32_t const BlockOverriddenNb;
    int32_t const BlockExponent;

    // Special block headers
    int32_t const StandardBlockHeader;
    int32_t const StandardBlockFloatSimmilar;
    int32_t const StandardBlockNumbersNoExp;
    int32_t const StandardSingleValue;
    int32_t const StandardBlockPatternOffset;
    int32_t const StandardBlockPatternSame;
    int32_t const StandardBlockPatternPingPong;

    HeaderSizes(BlockyMetadata const& metadata)
        : GlobalIsAbsoluteNegative(metadata.IsAbsolute ? 1 : 0)
        , BlockExponent(metadata.MaxNeededBitsExponent)
        , BlockIsAbsolute(metadata.IsAbsolute ? 0 : 1)
        , BlockIsAbsoluteNegative(BlockIsAbsolute)
        , BlockOverriddenNb(metadata.MaxNeededBitsNeededBitsNumber)
        , StandardSingleValue
        (
            IsBlock
          + GlobalNumberNb
          + (metadata.IsAbsolute ? 0 : Sign)
          + (metadata.NoExponent ? 0 : Sign + GlobalExponentNb)
        )
        , StandardBlockHeader
        (
            IsBlock
          + BlockHasExponent
          + BlockHasPattern
          + BlockLength
        )
        , StandardBlockFloatSimmilar
        (
            StandardBlockHeader
          + Sign
          + BlockExponent
          + BlockOverrideGlobalNb
          + BlockIsAbsolute
          + BlockIsAbsoluteNegative
        )
        , StandardBlockNumbersNoExp
        (
            StandardBlockHeader
          + BlockOverrideGlobalNb
          + BlockIsAbsolute
          + BlockIsAbsoluteNegative
        )
        , StandardBlockPatternOffset
        (
            StandardBlockHeader
          + (StandardSingleValue - IsBlock) * 2
          + BlockPatternType
        )
        , StandardBlockPatternSame
        (
            StandardBlockHeader
          + StandardSingleValue
          - IsBlock
          + BlockPatternType
        )
        , StandardBlockPatternPingPong(StandardBlockPatternOffset + 8) { }
};

#endif /* end of include guard: HEADERSIZES_HPP */
