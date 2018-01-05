#ifndef DECOMPRESSIONMETHOD_HPP
#define DECOMPRESSIONMETHOD_HPP

#include "../blockfinding/headersizes.hpp"
#include "../blockynumber.hpp"
#include "../bitwriter.hpp"
#include "../bitreader.hpp"
#include "../blockymetadata.hpp"
#include "../blockynumbersaver.hpp"
#include "methods.hpp"

#include "../block.hpp"

class DecompressionMethod
{
protected:
	size_t& index;
	BlockyMetadata const& metadata;

	DecompressionMethod
	(
		BlockyMetadata const& metadata,
		size_t& index
	)
		: metadata(metadata)
		, index(index) { }

	void write(BlockyNumberSaver& saver, BlockyNumber value);

public:
	virtual int read
	(
        BlockyNumberSaver& saver,
		BitReader& reader,
		Block block
	) = 0;

	static Block read_default_block_header
	(
		BitReader& reader,
		BlockyMetadata const& metadata
	);

	static BlockyNumber read_single_value_without_control_bit
	(
		BitReader& reader,
		BlockyMetadata const& metadata
	);
};


#endif /* end of include guard: DECOMPRESSIONMETHOD_HPP */
