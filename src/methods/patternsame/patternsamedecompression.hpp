#ifndef PATTERNSAMEDECOMPRESSION_HPP
#define PATTERNSAMEDECOMPRESSION_HPP

#include "../decompressionmethod.hpp"

class PatternSameDecompression
	: public DecompressionMethod
{
private:

public:
	PatternSameDecompression
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

#endif /* end of include guard: PATTERNSAMEDECOMPRESSION_HPP */