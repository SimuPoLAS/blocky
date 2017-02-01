#include <block.hpp>

Block::Block
(
    int32_t index,
    int16_t exponent,
    uint8_t nb,
    bool isSignNegative,
    int64_t biggestNumber,
    CompressionMethod const** methods,
    Methods const method,
    bool hasPattern
)
    : Index(index)
    , HasExponent(exponent != 0)
    , HasPattern(hasPattern)
    , Pattern(PatternType::Same)
    , Method(methods[(int32_t)method])
    , Exponent(exponent)
    , NeededBits(nb)
    , AbsoluteSign(true)
    , IsSignNegative(isSignNegative)
    , BiggestNumber(biggestNumber)
    , OverrideGlobalNb(false)
    , Length(0) { }

bool Block::is_valid()
{
    return Length > 0;
}

SavingGrade Block::savinggrade()
{
    if (HasPattern)
        return SavingGrade::Pattern;
    return HasExponent ? SavingGrade::Exp : SavingGrade::NoExp;
}

bool Block::should_override_nb(BlockyMetadata const&  metadata)
{
    return metadata.MaxNeededBitsNeededBitsNumber
      < (metadata.MaxNeededBitsNumber - NeededBits) * Length
     && !HasPattern;
}

int32_t Block::difference_with_nb
(
    BlockyMetadata const& metadata,
    uint8_t& newNb
)
{
    if
    (
        metadata.MaxNeededBitsNeededBitsNumber
      < (metadata.MaxNeededBitsNumber - newNb) * Length
    )
    {
        if (!OverrideGlobalNb)
        {
            OverrideGlobalNb = true;
            return (NeededBits - newNb) * Length
              - metadata.MaxNeededBitsNeededBitsNumber;
        }
    }
    else if (OverrideGlobalNb)
    {
        OverrideGlobalNb = false;
        newNb = metadata.MaxNeededBitsNumber;
        return (NeededBits - newNb) * Length
          + metadata.MaxNeededBitsNeededBitsNumber;
    }
    return (NeededBits - newNb) * Length;
}
