#include <methods/compressionmethod.hpp>

void CompressionMethod::write_default_blockheader
(
    BitWriter& writer,
    Block block
) const
{
    writer.write_byte(1, 1); // IsBlock
    if (block.HasExponent)
    {
        writer.write_byte(1, 1);
        writer.write_byte(uint8_t(block.Exponent < 0 ? 1 : 0), 1);
        writer.write
        (
            uint16_t(abs(block.Exponent)),
            metadata.MaxNeededBitsExponent
        );
    }
    else
        writer.write_byte(0, 1);

    if (block.HasPattern)
    {
        writer.write_byte(1, 1);
        writer.write_byte(uint8_t(block.Pattern), 2);
    }
    else
        writer.write_byte(0, 1);

    writer.write_byte(block.Length, 8);
}

void CompressionMethod::write_single_value_without_controlbit
(
    BitWriter& writer,
    shared_ptr<const BlockyNumber> value
) const
{
    if (!metadata.IsAbsolute)
        writer.write_byte(value->IsNegative ? 1 : 0, 1);
    writer.write(uint64_t(abs(value->Number)), metadata.MaxNeededBitsNumber);
    // Bug: potentually writing exponent even though NoExp is set to true
    writer.write_byte(value->Exponent < 0 ? 1 : 0, 1);
    writer.write
    (
        uint16_t(abs(value->Exponent)),
        metadata.MaxNeededBitsExponent
    );
}
