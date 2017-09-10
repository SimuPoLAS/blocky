#ifndef NUMBERSNOEXPDECOMPRESSION_HPP
#define NUMBERSNOEXPDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class NumbersNoExpDecompression 
	: public DecompressionMethod
{
private:

public:
	NumbersNoExpDecompression
	(
		BlockyMetadata const& metadata,
		std::vector<BlockyNumber>& values,
		size_t& index
	)
		: DecompressionMethod(metadata, values, index) { }

	int read
	(
		BitReader& reader,
		Block block
	) override;
};

#endif /* end of include guard: NUMBERSNOEXPDECOMPRESSION_HPP */