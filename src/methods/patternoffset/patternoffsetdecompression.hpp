#ifndef PATTERNOFFSETDECOMPRESSION_HPP
#define PATTERNOFFSETDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class PatternOffsetDecompression
	: public DecompressionMethod
{
private:

public:
	PatternOffsetDecompression
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

#endif /* end of include guard: PATTERNOFFSETDECOMPRESSION_HPP */