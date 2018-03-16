#include "numbersnoexpdecompression.hpp"

int NumbersNoExpDecompression::read
(
    BlockyNumberSaver& saver,
	BitReader & reader,
	Block block
)
{
	if (block.OverrideGlobalNb = reader.read_byte(1) > 0)
		block.IsSignNegative = reader.read_byte(1) > 0;

	auto has_absolute_sign = block.AbsoluteSign || metadata.IsAbsolute;
	auto is_negative = (block.AbsoluteSign && block.IsSignNegative) || (metadata.IsAbsolute && metadata.IsNegative);

	for (size_t i = 0; i < block.Length; i++)
	{
		if (!has_absolute_sign)
			is_negative = reader.read_byte(1) > 0;

		int64_t number = ((int64_t)reader.read(metadata.MaxNeededBitsNeededBitsNumber)) * (is_negative ? -1 : 1);
		// TODO: check whether this is actually real? no idea how to interact with blockynumber
		// when it's a number with no exp
		BlockyNumber value = BlockyNumber(number, 0);

		std::cout << "[numbersnoexpdecompression]" << "\n" << value.to_s() << "\n";
		saver.write(value);
	}

	return block.Length;
}
