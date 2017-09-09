#ifndef PATTERNOFFSETDECOMPRESSION_HPP
#define PATTERNOFFSETDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class PatternOffsetDecompression
	: public DecompressionMethod
{
private:

private:
	PatternOffsetDecompression
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

#endif /* end of include guard: PATTERNOFFSETDECOMPRESSION_HPP */