#ifndef NUMBERSNOEXPDECOMPRESSION_HPP
#define NUMBERSNOEXPDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class NumbersNoExpDecompression 
	: public DecompressionMethod
{
private:

private:
	NumbersNoExpDecompression
	(
		BlockyMetadata const& metadata
	) 
		: DecompressionMethod(metadata) { }

	int read
	(
		BitReader& reader,
		BitWriter& writer,
		Block block
	) override;
};

#endif /* end of include guard: NUMBERSNOEXPDECOMPRESSION_HPP */