#include <math.h>
#include <algorithm>

#include "methods/floatsimmilar/floatsimmilarcompression.hpp"

using namespace std;

bool FloatSimmilarCompression::process_value
(
    Block& block,
    BlockyNumber const& value,
    int32_t index,
    int32_t& bitDiff
) const
{
    if (block.Length == 255)
        return false;

    if (block.Exponent < value.Exponent)
    {
        auto expDiff = value.Exponent - block.Exponent;

        if (expDiff > 18)
            // long can not hold 10^19 or higher
            return false;

        auto multiplier = int64_t(pow(10, expDiff));
        auto newNum = value.Number;
        if (value.Number != 0)
        {
            if (metadata.LargestPossibleValue / value.Number < multiplier)
                // overflow check
                return false;
            newNum = value.Number * multiplier;
        }

        auto newNb = uint8_t(floor(log2(abs(newNum))) + 1);
        if (value.NeededBitsNumber > metadata.MaxNeededBitsNumber)
            return false;

        // Not needed because value is nether reference type
        // nor used later => just commenting this lines for now
        //value.Exponent -= int16_t(expDiff);
        //value.NeededBitsNumber = newNb;
        //value.Number = newNum;

        if (newNb > block.NeededBits)
        {
            auto nbNewNumber = newNb;
            bitDiff += block.difference_with_nb(metadata, nbNewNumber);
            block.NeededBits = nbNewNumber;
        }
    }
    else if (block.Exponent > value.Exponent)
    {
        auto expDiff = (block.Exponent - value.Exponent);

        if (expDiff > 18)
            return false; // int64 can't hold 10^19 or higher
        auto multiplier = int64_t(pow(10, expDiff));

        if
        (
            block.BiggestNumber > 0
         && int64_t
            (
                metadata.LargestPossibleValue
              / block.BiggestNumber
            )
          < multiplier
        )
            return false; // overflow check
        auto newNum = block.BiggestNumber * multiplier;

        block.BiggestNumber = value.Number > newNum ? value.Number : newNum;
        block.Exponent = value.Exponent;

        auto bigNumNb = max
        (
            uint8_t(floor(log2(abs(block.BiggestNumber))) + 1),
            value.NeededBitsNumber
        );
        if (bigNumNb > metadata.MaxNeededBitsNumber)
            return false;

        if (bigNumNb > block.NeededBits)
        {
            auto diff = block.difference_with_nb(metadata, bigNumNb);
            bitDiff += diff;
            block.NeededBits = bigNumNb;
        }
    }

    if (block.AbsoluteSign)
    {
        if (block.IsSignNegative != value.IsNegative)
        {
            block.AbsoluteSign = false;
            bitDiff -= block.Length - 1;
        }
        else
            bitDiff++;
    }

    if (value.Number > block.BiggestNumber)
    {
        block.BiggestNumber = value.Number;
        if (block.NeededBits < value.NeededBitsNumber)
        {
            auto nbNewBiggest = value.NeededBitsNumber;
            bitDiff += block.difference_with_nb(metadata, nbNewBiggest);
            block.NeededBits = nbNewBiggest;
        }
    }

    block.Length++;

    bitDiff += metadata.MaxNeededBitsExponent + 2;
    if (block.OverrideGlobalNb)
        bitDiff += metadata.MaxNeededBitsNumber - block.NeededBits;
    return true;
}

void FloatSimmilarCompression::write
(
    BitWriter writer,
    Block block,
    int32_t& valueIndex
) const
{
    write_default_blockheader(writer, block);

    if (block.OverrideGlobalNb)
    {
        writer.write_byte(1, 1);
        writer.write
        (
            metadata.MaxNeededBitsNumber,
            metadata.MaxNeededBitsNeededBitsNumber
        );
    }
    else
        writer.write_byte(0, 1);

    if (!metadata.IsAbsolute)
    {
        if (block.AbsoluteSign)
        {
            writer.write_byte(1, 1);
            writer.write_byte(block.IsSignNegative ? 1 : 0, 1);
        }
        else
            writer.write_byte(0, 1);
    }

    auto end = block.Index + block.Length;
    for (size_t i = block.Index; i < end; i++)
    {
        auto value = values[i];

        if (!block.AbsoluteSign)
            writer.write_byte(value.IsNegative ? 1 : 0, 1);

        if (value.Exponent > block.Exponent)
            writer.write
            (
                uint64_t
                (
                    pow(10, value.Exponent - block.Exponent)
                  * value.Number
                ),
                metadata.MaxNeededBitsNumber
            );
        else
            writer.write(uint64_t(value.Number), metadata.MaxNeededBitsNumber);
    }
    valueIndex += block.Length;
}
