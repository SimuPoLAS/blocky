#include <block.hpp>

Block::Block
(
    int32_t index,
    int16_t exponent,
    uint8_t nb,
    bool isSignNegative,
    int64_t biggestNumber,
    CompressionMethod** methods,
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

SavingGrade Block::savinggrade() const
{
    if (HasPattern)
        return SavingGrade::Pattern;
    return HasExponent ? SavingGrade::Exp : SavingGrade::NoExp;
}

bool Block::should_override_nb(BlockyMetadata const&  metadata) const
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

Block& Block::operator=(Block const& other)
{
    Index = other.Index;
    HasExponent = other.HasExponent;
    HasPattern = other.HasPattern;
    Pattern = other.Pattern;

    Exponent = other.Exponent;
    OverrideGlobalNb = other.OverrideGlobalNb;
    NeededBits = other.NeededBits;
    Length = other.Length;
    AbsoluteSign = other.AbsoluteSign;
    IsSignNegative = other.IsSignNegative;
    BiggestNumber = other.BiggestNumber;
    IsValid = other.IsValid;

    Method = other.Method;
}
