#ifndef PATTERNPINGPONGDECOMPRESSION_HPP
#define PATTERNPINGPONGDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class PatternPingPongDecompression
	: public DecompressionMethod
{
private:

public:
	PatternPingPongDecompression
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

#endif /* end of include guard: PATTERNPINGPONGDECOMPRESSION_HPP */