#include "patternpingpongdecompression.hpp"

int PatternPingPongDecompression::read
(
    BlockyNumberSaver& saver,
	BitReader & reader,
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
		std::cout << "[patterndingdongdecompression]" << "\n" << value.to_s() << "\n";
		saver.write(value);
	}
	return 0;
}
