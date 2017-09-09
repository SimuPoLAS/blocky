#ifndef FLOATSIMILARDECOMPRESSION_HPP
#define FLOATSIMILARDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class FloatSimilarDecompression
	: public DecompressionMethod
{
private:

public:
	FloatSimilarDecompression
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

#endif /* end of include guard: FLOATSIMILARDECOMPRESSION_HPP */