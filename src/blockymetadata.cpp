#include <math.h>
#include <iostream>
#include <string>

#include <blockymetadata.hpp>

void BlockyMetadata::write(BitWriter& writer)
{
    std::cout << "ValueCount: " << ValueCount << '\n';
    std::cout << "IsAbsolute: " << IsAbsolute << '\n';
    std::cout << "IsNegative: " << IsNegative << '\n';
    std::cout << "MaxNeededBitsNumber: " << to_string(MaxNeededBitsNumber) << '\n';
    std::cout << "MaxNeededBitsExponent: " << to_string(MaxNeededBitsExponent) << '\n';

    writer.write(uint64_t(ValueCount), 31);
    writer.write_byte(IsAbsolute ? uint8_t(1) : uint8_t(0), 1);
    if (IsAbsolute)
        writer.write_byte(IsNegative ? uint8_t(1) : uint8_t(0), 1);
    writer.write_byte(MaxNeededBitsExponent, 4);
    writer.write_byte(MaxNeededBitsNumber, 6);
}

BlockyMetadata BlockyMetadata::from_bit_stream(BitReader& reader)
{
    BlockyMetadata metadata;
    metadata.ValueCount = int32_t(reader.read(31));
    if (metadata.IsAbsolute = reader.read_byte(1) > 0)
        metadata.IsNegative = reader.read_byte(1) > 0;
    metadata.MaxNeededBitsExponent = reader.read_byte(4);
    metadata.MaxNeededBitsNumber = reader.read_byte(6);
    metadata.MaxNeededBitsNeededBitsNumber =
        uint8_t(floor(log2(metadata.MaxNeededBitsNumber)) + 1);
    metadata.LargestPossibleValue = pow(2, metadata.MaxNeededBitsNumber);
    return metadata;
}

BlockyMetadata BlockyMetadata::from_data
(
    shared_ptr<BlockyNumber>* values,
    size_t n
)
{
    if (n == 0)
        // TODO: Throw a specified for this case value (not just 0)
        throw 0;
    BlockyMetadata metadata;
    metadata.IsNegative = values[0]->IsNegative;
    metadata.IsAbsolute = true;
    metadata.ValueCount = n;
    for (size_t i = 0; i < n; i++)
    {
        auto number = values[i];
        if (number->NeededBitsNumber > metadata.MaxNeededBitsNumber)
        {
            metadata.MaxNeededBitsNumber = number->NeededBitsNumber;
            metadata.LargestPossibleValue = pow(2, number->NeededBitsNumber);
        }
        if (number->NeededBitsExponent > metadata.MaxNeededBitsExponent)
            metadata.MaxNeededBitsExponent = number->NeededBitsExponent;
        if (metadata.IsAbsolute && number->IsNegative != metadata.IsNegative)
            metadata.IsAbsolute = false;
        if (metadata.NoExponent && number->Exponent != 0)
            metadata.NoExponent = false;
    }

    std::cout << "from_data: MaxNeededBitsNumber: " << to_string(metadata.MaxNeededBitsNumber) << '\n';

    return metadata;
}
