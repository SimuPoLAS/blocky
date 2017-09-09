#include "patternpingpongdecompression.hpp"

int PatternPingPongDecompression::read
(
	BitReader & reader, 
	BitWriter & writer, 
	Block block
)
{
	BlockyNumber value1 = read_single_value_without_control_bit(reader, metadata);
	BlockyNumber value2 = read_single_value_without_control_bit(reader, metadata);

	auto pp_length = reader.read_byte(8);
	auto total = pp_length * block.Length;

	for (size_t i = 0; i < total; i++)
	{
		BlockyNumber& value = i % (pp_length * 2) >= pp_length ? value2 : value1;
		std::cout << "[patterndingdongdecompression]" << "\n" << value.to_s << "\n";
		writer.write(value, value.NeededBitsNumber);
	}
	return 0;
}