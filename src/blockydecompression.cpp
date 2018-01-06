#include "blockydecompression.hpp"

#include "methods/floatsimilar/floatsimilardecompression.hpp"
#include "methods/numbersnoexp/numbersnoexpdecompression.hpp"
#include "methods/patternoffset/patternoffsetdecompression.hpp"
#include "methods/patternpingpong/patternpingpongdecompression.hpp"
#include "methods/patternsame/patternsamedecompression.hpp"

BlockyDecompression::BlockyDecompression(LZMAFILE* data, BlockyNumberSaver& saver, BitReader& reader, size_t size = 1)
    : reader(reader), saver(saver), size(size)
{
    // TODO: get this to work
    metadata = BlockyMetadata::from_bit_stream(reader);

    methods[(int) Methods::PatternSame] = new PatternOffsetDecompression(metadata, index);
    methods[(int) Methods::PatternPingPong] = new PatternPingPongDecompression(metadata, index);
    methods[(int) Methods::FloatSimilar] = new FloatSimilarDecompression(metadata, index);
    methods[(int) Methods::NumbersNoExp] = new NumbersNoExpDecompression(metadata, index);
    methods[(int) Methods::PatternOffset] = new PatternOffsetDecompression(metadata, index);

    if (reader.read_byte(1) > 0) // use huffman (xd)
        // TODO: meaningful exception (ecksdee)
        exit(-501);
}

BlockyDecompression::~BlockyDecompression()
{
    for (size_t i = 0; i < METHODS_COUNT; i++)
        delete methods[i];
}

DecompressionMethod* BlockyDecompression::get_method_for_block(Block block)
{
    if (!block.HasPattern) {
        return block.HasExponent
            ? (methods[(int)Methods::FloatSimilar])
            : (methods[(int)Methods::NumbersNoExp]);
    }

    switch (block.Pattern)
    {
    case PatternType::Same:
        return methods[(int)Methods::PatternSame];
    case PatternType::Offset:
        return methods[(int)Methods::PatternOffset];
    case PatternType::Pingpong:
        return methods[(int)Methods::PatternPingPong];
    case PatternType::Reserved:
        // TODO: throw meaningful exception instead of not implemented error code
        std::cout << "invalid pattern type" << "\n";
        exit(-501);
    default:
        // TODO: throw meaningful exception instead of not implemented error code
        std::cout << "pattern type not implemented" << "\n";
        exit(-501);
    }
}

void BlockyDecompression::decompress()
{
    size_t value_count = 0;
    while (value_count < metadata.ValueCount)
    {
        if (reader.read_byte(1) > 0) // isBlock
        {
            // TODO: evaluate safety of pointer arithmetic
            Block block = DecompressionMethod::read_default_block_header(reader, metadata);
            DecompressionMethod* method = get_method_for_block(block);
            value_count += method->read(saver, reader, block);
        }
        else
        {
            BlockyNumber value = DecompressionMethod::read_single_value_without_control_bit(reader, metadata);
            write(value);
            value_count++;
        }
    }
}

void BlockyDecompression::write(BlockyNumber value)
{
    std::cout << "[blockydecompression]" << "\n" << value.to_s() << "\n";
    saver.write(value);
}