#include "decompressionmethod.hpp"

void DecompressionMethod::write(BlockyNumberSaver& saver, BlockyNumber value)
{
	std::cout << "[decompressionmethod] write called" << "\n";
	saver.values[index++] = value;
}

// Exclusive IsBlock
Block DecompressionMethod::read_default_block_header
(
	BitReader& reader,
	BlockyMetadata const& metadata
)
{
	Block block = Block();

	if (block.HasExponent = reader.read_byte(1) > 0)
	{
		auto negative = reader.read_byte(1) > 0;
		block.Exponent = (uint16_t) reader.read(metadata.MaxNeededBitsExponent);
		if (negative)
			block.Exponent *= -1;
	}

	if (block.HasPattern = reader.read_byte(1) > 0)
	{
		block.Pattern = (PatternType) reader.read_byte(2);
	}

	block.Length = reader.read_byte(8);
	return block;
}

// Reads a single value from its binary representation without the control bit (isBlock)
BlockyNumber DecompressionMethod::read_single_value_without_control_bit
(
	BitReader& reader,
	BlockyMetadata const& metadata
)
{
	bool is_negative;
	if (!metadata.IsAbsolute)
		is_negative = reader.read_byte(1) > 0;
	else
		is_negative = metadata.IsNegative;

	int64_t number = (int64_t)reader.read(metadata.MaxNeededBitsNumber);

	bool is_exp_negative = reader.read_byte(1) > 0;
	// Bug (taken from Ofc): 
	// may potentially read exp even though no exp is set
	// same in the writing method!
	int16_t exp = (int16_t)reader.read(metadata.MaxNeededBitsExponent);
	if (is_exp_negative)
		exp *= -1;

	BlockyNumber value = BlockyNumber(number, exp);
	value.IsNegative = is_negative;
	return value;
}
