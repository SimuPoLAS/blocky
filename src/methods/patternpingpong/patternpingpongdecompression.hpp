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

#endif /* end of include guard: PATTERNPINGPONGDECOMPRESSION_HPP */