#include "blockydecompression.hpp"

#include "methods/floatsimilar/floatsimilardecompression.hpp"
#include "methods/numbersnoexp/numbersnoexpdecompression.hpp"
#include "methods/patternoffset/patternoffsetdecompression.hpp"
#include "methods/patternpingpong/patternpingpongdecompression.hpp"
#include "methods/patternsame/patternsamedecompression.hpp"

BlockyDecompression::BlockyDecompression(FILE* file)
	: reader(BitReader(file))
{
	metadata = BlockyMetadata::from_bit_stream(reader);

	methods[(int)Methods::PatternSame] = new PatternOffsetDecompression(metadata, values, index);
	methods[(int)Methods::PatternPingPong] = new PatternPingPongDecompression(metadata, values, index);
	methods[(int)Methods::FloatSimilar] = new FloatSimilarDecompression(metadata, values, index);
	methods[(int)Methods::NumbersNoExp] = new NumbersNoExpDecompression(metadata, values, index);
	methods[(int)Methods::PatternOffset] = new PatternOffsetDecompression(metadata, values, index);

	if (reader.read_byte(1) > 0) // use huffman (xd)
		// TODO: meaningful exception (ecksdee)
		exit(-501);
}

/*
BlockyDecompression::BlockyDecompression(FILE * file, MarerReporter reporter)
	: reader(BitReader(file)), reporter(reporter)
{
	
}
*/

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
		// so to get things clear, this method is actually responsible
		// for the conversion of our data (in form of blockynumbers) to
		// strings that are written directly to the output file

		// please don't implement rudimentary code before actually checking
		// how this usually gets implemented in cpp projects

		// for future reference: 
		// https://github.com/SimuPoLAS/Ofc/blob/master/src/Ofc/IO/MarerReader.cs#L131
		switch (reporter_get_size())
		{
		case 1:
			// TODO: check how to actually use write
			// ofc: _writer.WriteLine(_buffer[0]);
			//writer.write(buffer[0],)
		default:
			break;
		}
		reporter_position = 0;
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
