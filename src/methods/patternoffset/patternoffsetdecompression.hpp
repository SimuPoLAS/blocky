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

#endif /* end of include guard: PATTERNOFFSETDECOMPRESSION_HPP */