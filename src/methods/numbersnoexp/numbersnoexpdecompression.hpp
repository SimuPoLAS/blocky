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
		size_t& index
	)
		: DecompressionMethod(metadata, index) { }

	int read
	(
        BlockyNumberSaver& saver,
		BitReader& reader,
		Block block
	) override;
};

#endif /* end of include guard: NUMBERSNOEXPDECOMPRESSION_HPP */