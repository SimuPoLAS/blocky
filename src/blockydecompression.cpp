#include "blockydecompression.hpp"

#include "methods/floatsimilar/floatsimilardecompression.hpp"
#include "methods/numbersnoexp/numbersnoexpdecompression.hpp"
#include "methods/patternoffset/patternoffsetdecompression.hpp"
#include "methods/patternpingpong/patternpingpongdecompression.hpp"
#include "methods/patternsame/patternsamedecompression.hpp"

BlockyDecompression::BlockyDecompression(FILE* file)
{
	reader = BitReader(file);
	metadata = BlockyMetadata::from_bit_stream(reader);

	methods[(int)Methods::PatternSame] = new PatternOffsetDecompression(metadata);
	methods[(int)Methods::PatternPingPong] = new PatternPingPongDecompression(metadata);
	methods[(int)Methods::FloatSimilar] = new FloatSimilarDecompression(metadata);
	methods[(int)Methods::NumbersNoExp] = new NumbersNoExpDecompression(metadata);
	methods[(int)Methods::PatternOffset] = new PatternOffsetDecompression(metadata);

	if (reader.read_byte(1) > 0) // use huffman (xd)
		// TODO: meaningful exception (ecksdee)
		exit(-501);
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
	values = new BlockyNumber[value_count];
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
			value_count += method->read(reader, writer, block);
		}
		else
		{
			BlockyNumber value = DecompressionMethod::read_single_value_without_control_bit(reader, metadata);
			writer.write(value, value);
			value_count++;
		}
	}
}

void BlockyDecompression::write(BlockyNumber value)
{
	std::cout << "[blockydecompression] write called" << "\n";
	values[index++] = value;
}
