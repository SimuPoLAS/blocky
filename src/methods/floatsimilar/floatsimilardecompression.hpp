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

#endif /* end of include guard: FLOATSIMILARDECOMPRESSION_HPP */