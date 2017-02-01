#ifndef BLOCKYMETADATA_HPP
#define BLOCKYMETADATA_HPP

#include <vector>

#include <bitreader.hpp>
#include <bitwriter.hpp>
#include <blockynumber.hpp>

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

    void write(BitWriter& writer);

    static BlockyMetadata from_bit_stream(BitReader& reader);
    static BlockyMetadata from_data(BlockyNumber* values, size_t n);
};

#endif /* end of include guard: BLOCKYMETADATA_HPP */
