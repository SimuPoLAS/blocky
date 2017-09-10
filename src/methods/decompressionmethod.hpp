#ifndef DECOMPRESSIONMETHOD_HPP
#define DECOMPRESSIONMETHOD_HPP

#include "../blockfinding/headersizes.hpp"
#include "../blockynumber.hpp"
#include "../bitwriter.hpp"
#include "../bitreader.hpp"
#include "../blockymetadata.hpp"
#include "methods.hpp"

#include "../block.hpp"

class DecompressionMethod
{
protected:
	std::vector<BlockyNumber>& values;
	size_t& index;
	BlockyMetadata const& metadata;

	DecompressionMethod
	(
		BlockyMetadata const& metadata,
		std::vector<BlockyNumber>& values,
		size_t& index
	)
		: metadata(metadata)
		, values(values)
		, index(index) { }

	void write(BlockyNumber value);

public:
	virtual int read
	(
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
