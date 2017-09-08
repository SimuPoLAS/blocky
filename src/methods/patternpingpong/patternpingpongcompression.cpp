#include "patternpingpongcompression.hpp"

bool PatternPingPongCompression::process_value
(
    Block& block,
    const BlockyNumber& value,
    int32_t index,
    int32_t& bitDiff
)
{
    // TODO: throw meaningful exception, not just zero
    throw 0;
}

void PatternPingPongCompression::write
(
    BitWriter& writer,
    Block block,
    size_t& valueIndex
)
{
    write_default_blockheader(writer, block);

    if (probabilityMetadataIndex > PingPongPatternLengths.size())
        probabilityMetadataIndex = 0;

    auto meta = PingPongPatternLengths[probabilityMetadataIndex++];
    if (meta.BlockIndex != block.Index)
    {
        for (size_t i = 0; i < PingPongPatternLengths.size(); i++)
        {
            if (PingPongPatternLengths[i].BlockIndex == block.Index)
            {
                meta = PingPongPatternLengths[i];
                probabilityMetadataIndex = i++;
                break;
            }
        }
    }

    write_single_value_without_controlbit
    (
        writer,
        *values[block.Index]
    );
    write_single_value_without_controlbit
    (
        writer,
        *values[block.Index + meta.Length]
    );

    writer.write_byte(meta.Length, 8);
    valueIndex += meta.Length * block.Length;
}
