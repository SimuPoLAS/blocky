#ifndef BLOCKYMETADATA_HPP
#define BLOCKYMETADATA_HPP

class BlockyMetadata
{
private:
    BlockyMetadata() { }

public:
    int32_t ValueCount;
    uint8_t MaxNeededBitsNumber;
    uint8_t MaxNeededBitsNeededBitsNumber;
    uint8_t MaxNeededBitsExponent;
    bool IsAbsolute;
    bool IsNegative;
    bool NoExponent;
    uint64_t LargestPossibleValue;

    static BlockyMetadata from_bit_stream()
};

#endif /* end of include guard: BLOCKYMETADATA_HPP */
