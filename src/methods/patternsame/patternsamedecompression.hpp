#ifndef PATTERNSAMEDECOMPRESSION_HPP
#define PATTERNSAMEDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class PatternSameDecompression
	: public DecompressionMethod
{
private:

private:
	PatternSameDecompression
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

#endif /* end of include guard: PATTERNSAMEDECOMPRESSION_HPP */