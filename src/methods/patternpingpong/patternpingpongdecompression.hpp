#ifndef PATTERNPINGPONGDECOMPRESSION_HPP
#define PATTERNPINGPONGDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class PatternPingPongDecompression
	: public DecompressionMethod
{
private:

private:
	PatternPingPongDecompression
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

#endif /* end of include guard: PATTERNPINGPONGDECOMPRESSION_HPP */