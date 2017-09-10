#ifndef PATTERNSAMEDECOMPRESSION_HPP
#define PATTERNSAMEDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class PatternSameDecompression
	: public DecompressionMethod
{
private:

public:
	PatternSameDecompression
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

#endif /* end of include guard: PATTERNSAMEDECOMPRESSION_HPP */