#include "blockydecompression.hpp"

#include "methods/floatsimilar/floatsimilardecompression.hpp"
#include "methods/numbersnoexp/numbersnoexpdecompression.hpp"
#include "methods/patternoffset/patternoffsetdecompression.hpp"
#include "methods/patternpingpong/patternpingpongdecompression.hpp"
#include "methods/patternsame/patternsamedecompression.hpp"

/*
BlockyDecompression::BlockyDecompression(FILE* file)
    : reader(BitReader(file)), file(file)
{
    metadata = BlockyMetadata::from_bit_stream(reader);
    reporter_set_size(1);

    methods[(int)Methods::PatternSame] = new PatternOffsetDecompression(metadata, values, index);
    methods[(int)Methods::PatternPingPong] = new PatternPingPongDecompression(metadata, values, index);
    methods[(int)Methods::FloatSimilar] = new FloatSimilarDecompression(metadata, values, index);
    methods[(int)Methods::NumbersNoExp] = new NumbersNoExpDecompression(metadata, values, index);
    methods[(int)Methods::PatternOffset] = new PatternOffsetDecompression(metadata, values, index);

    if (reader.read_byte(1) > 0) // use huffman (xd)
        // TODO: meaningful exception (ecksdee)
        exit(-501);
}
*/

BlockyDecompression::BlockyDecompression(LZMAFILE* data, char* buffer, BlockyNumberSaver decomp, BitReader& reader)
    // temporary
    : data(data), reader(reader)
{
    // TODO: get this to work
    metadata = BlockyMetadata::from_bit_stream(reader);
    reporter_set_size(1);

    methods[(int) Methods::PatternSame] = new PatternOffsetDecompression(metadata, values, index);
    methods[(int) Methods::PatternPingPong] = new PatternPingPongDecompression(metadata, values, index);
    methods[(int) Methods::FloatSimilar] = new FloatSimilarDecompression(metadata, values, index);
    methods[(int) Methods::NumbersNoExp] = new NumbersNoExpDecompression(metadata, values, index);
    methods[(int) Methods::PatternOffset] = new PatternOffsetDecompression(metadata, values, index);

    if (reader.read_byte(1) > 0) // use huffman (xd)
        // TODO: meaningful exception (ecksdee)
        exit(-501);
}

BlockyDecompression::~BlockyDecompression()
{
    //delete file;
    for (size_t i = 0; i < METHODS_COUNT; i++)
        delete methods[i];
}

size_t BlockyDecompression::reporter_get_size()
{
    return buffer.size();
}

void BlockyDecompression::reporter_set_size(size_t size)
{
    buffer.resize(size);
    reporter_position = 0;
}

std::unique_ptr<DecompressionMethod> BlockyDecompression::get_method_for_block
(
    Block block
)
{
    if (!block.HasPattern) return block.HasExponent 
        ? std::unique_ptr<DecompressionMethod>(methods[(int)Methods::FloatSimilar]) 
            : std::unique_ptr<DecompressionMethod>(methods[(int)Methods::NumbersNoExp]);

    switch (block.Pattern)
    {
    case PatternType::Same:
        return std::unique_ptr<DecompressionMethod>(methods[(int)Methods::PatternSame]);
    case PatternType::Offset:
        return std::unique_ptr<DecompressionMethod>(methods[(int)Methods::PatternOffset]);
    case PatternType::Pingpong:
        return std::unique_ptr<DecompressionMethod>(methods[(int)Methods::PatternPingPong]);
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

void BlockyDecompression::initialize(int value_count)
{
    values.resize(value_count);
}

void BlockyDecompression::decompress()
{
    size_t value_count = 0;
    while (value_count < metadata.ValueCount)
    {
        if (reader.read_byte(1) > 0) // isBlock
        {
            Block block = DecompressionMethod::read_default_block_header(reader, metadata);
            auto method = get_method_for_block(block);
            value_count += method->read(reader, block);
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
    std::cout << "[blockydecompression] write called" << "\n";
    values[index++] = value;
}

void BlockyDecompression::report(BlockyNumber value)
{
    buffer[reporter_position++] = value.to_s();
    if (reporter_position == buffer.size())
    {
        // for future reference: 
        // https://github.com/SimuPoLAS/Ofc/blob/master/src/Ofc/IO/MarerReader.cs#L131
        //std::ofstream out(file);
        std::stringstream ss;

        switch (reporter_get_size())
        {
        case 1:
            //fwrite(buffer.data(), sizeof(char), buffer.size(), file);
            //out << buffer[0];
        case 3:
            // what did the femtolas team mean by this? we may never find out
        case 9:
            // strongest number 9

            ss << "(";
            // concatenate strings in buffer
            // TODO: check that the efficiency of the copy approach isn't garbage
            // (at the very least, this isn't the worst method there is)
            //copy
            //(
            //    buffer.begin(), 
            //    buffer.end(), 
            //    ostream_iterator<int>(ss, " ")
            //);
            ss << ")";

            //fwrite(ss.str().data(), sizeof(char), ss.str().size(), file);
            //out << ss.str();
            break;
        default:
            // TODO: throw meaningful exception instead of not implemented error code
            exit(-501);
        }
        reporter_position = 0;
        //out.close();
    }
}

void BlockyDecompression::report(BlockyNumber* values, size_t offset, size_t amount)
{
    for (size_t i = offset; i < offset + amount; i++)
        report(values[i]);
}

void BlockyDecompression::finish()
{
    // do nothing and be useless, just like in ofc :)
}
