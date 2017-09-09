#include "blockydecompression.hpp"

#include "methods/floatsimilar/floatsimilardecompression.hpp"
#include "methods/numbersnoexp/numbersnoexpdecompression.hpp"
#include "methods/patternoffset/patternoffsetdecompression.hpp"
#include "methods/patternpingpong/patternpingpongdecompression.hpp"
#include "methods/patternsame/patternsamedecompression.hpp"

DecompressionMethod BlockyDecompression::get_method_for_block
(
	Block block
)
{
	if (!block.HasPattern) return block.HasExponent ? methods[(int)Methods::FloatSimilar] : methods[(int)Methods::NumbersNoExp];

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
			Block block = DecompressionMethod::read_default_block_header(reader, metadata);
			auto method = get_method_for_block(block);
			value_count += method.read(reader, writer, block);
		}
		else
		{
			BlockyNumber value = DecompressionMethod::read_single_value_without_control_bit(reader, metadata);
			writer.write(value, value.NeededBitsNumber);
			value_count++;
		}
	}
}

void BlockyDecompression::write
(
	BlockyNumber value
)
{
	std::cout << "[blockydecompression] write called" << "\n";
	writer.write(value, value.NeededBitsNumber);
}
