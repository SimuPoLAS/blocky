#include "patternoffsetdecompression.hpp"

int PatternOffsetDecompression::read
(
	BitReader & reader, 
	BitWriter & writer, 
	Block block
)
{
	BlockyNumber value1 = read_single_value_without_control_bit(reader, metadata);
	BlockyNumber value2 = read_single_value_without_control_bit(reader, metadata);

	std::cout << "[patternoffsetdecompression] bn[1]" << "\n" << value1.to_s << "\n";
	writer.write(value1, value1.NeededBitsNumber);
	std::cout << "[patternoffsetdecompression] bn[2]" << "\n" << value2.to_s << "\n";
	writer.write(value2, value2.NeededBitsNumber);

	BlockyNumber diff = value2.subtract_each(value1);

	for (size_t i = 0; i < block.Length; i++)
	{
		BlockyNumber bni = diff.linear_multiply_each(i).add_each(value2);
		std::cout << "[patternoffsetdecompression] bn[i]" << "\n" << bni.to_s << "\n";
		writer.write(bni, bni.NeededBitsNumber);
	}

	return block.Length;
}
