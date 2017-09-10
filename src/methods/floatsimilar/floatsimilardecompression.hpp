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

#endif /* end of include guard: FLOATSIMILARDECOMPRESSION_HPP */