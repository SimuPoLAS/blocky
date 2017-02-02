#include <methods/numbersnoexp/numbersnoexpcompression.hpp>

bool NummbersNoExpCompression::process_value
(
    Block& block,
    BlockyNumber const& value,
    int32_t index,
    int32_t& bitDiff
)
{
    if (value.Exponent != 0)
        return false; //Todo: maybe not impossible?

    if (block.AbsoluteSign)
    {
        if (block.IsSignNegative != value.IsNegative) // Check if the new value works with the current "absolute sign" block header
        {
            block.AbsoluteSign = false;
            bitDiff -= block.Length - 1; // We loose 1 bit per value, because we need to write down the sign now ... but we save 1 because less block header stuffs
        }
        else
            bitDiff++;
    }

    if (value.Number > block.BiggestNumber) // If the biggest value in the block is smaller than the new one, we need to set it for future calculations to be correct
    {
        block.BiggestNumber = value.Number;
        if (block.NeededBits < value.NeededBitsNumber) // If the new number needs more bits than specified in the block header, we need to adjust that
        {
            auto nbNewBiggest = value.NeededBitsNumber;
            bitDiff += block.difference_with_nb(metadata, nbNewBiggest); // Adds the difference in bits that comes with changing the block header. May change bigNumNb to the global-header-max! (can be worth, because no header then ...)
            block.NeededBits = nbNewBiggest; // Need to set this after the call for future calculations to be exact
        }
    }

    block.Length++;

    bitDiff += metadata.MaxNeededBitsExponent + 1;
    if (block.OverrideGlobalNb)
        bitDiff += metadata.MaxNeededBitsNumber - block.NeededBits;
    return true;
}


void NummbersNoExpCompression::write
(
    BitWriter writer,
    Block block,
    int32_t& valueIndex
)
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
    for (auto i = block.Index; i < end; i++)
    {
        auto value = values[i];

        if (!block.AbsoluteSign)
            writer.write_byte(value.IsNegative ? 1 : 0, 1);

        writer.write(uint64_t(value.Number), metadata.MaxNeededBitsNumber);
    }
    valueIndex += block.Length;
}
