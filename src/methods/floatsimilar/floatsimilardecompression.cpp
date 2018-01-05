#include "floatsimilardecompression.hpp"

int FloatSimilarDecompression::read
(
    BlockyNumberSaver& saver,
	BitReader & reader,
	Block block
)
{
	if (block.OverrideGlobalNb = reader.read_byte(1) > 0)
		block.NeededBits = reader.read_byte(metadata.MaxNeededBitsNeededBitsNumber);

	if (!metadata.IsAbsolute)
		if (block.AbsoluteSign = reader.read_byte(1) > 0)
			block.IsSignNegative = reader.read_byte(1) > 0;

	auto has_absolute_sign = block.AbsoluteSign || metadata.IsAbsolute;
	auto is_negative = (block.AbsoluteSign && block.IsSignNegative) || (metadata.IsAbsolute && metadata.IsNegative);
	
	for (size_t i = 0; i < block.Length; i++)
	{
		int64_t number = ((int64_t)reader.read(metadata.MaxNeededBitsNeededBitsNumber)) * (is_negative ? -1 : 1);
		int16_t exp = block.Exponent;
		auto value = BlockyNumber(number, exp);
		// TODO: how the heck does one use write
		// AAAAAAAAA ?
		std::cout << "[floatsimilardecompression]" << "\n" << value.to_s() << "\n";
		saver.write(value);
	}

	return block.Length;
}
