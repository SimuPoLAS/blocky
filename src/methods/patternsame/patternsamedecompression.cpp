#include "patternsamedecompression.hpp"

int PatternSameDecompression::read
(
	BitReader & reader,
	Block block
)
{
	BlockyNumber value = read_single_value_without_control_bit(reader, metadata);
	for (size_t i = 0; i < block.Length; i++)
	{
		std::cout << "[patternsamedecompression]" << "\n" << value.to_s() << "\n";
		write(value);
	}
	return 0;
}
