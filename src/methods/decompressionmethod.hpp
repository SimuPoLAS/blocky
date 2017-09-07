#ifndef DECOMPRESSIONMETHOD_HPP
#define DECOMPRESSIONMETHOD_HPP

#include <blockfinding/headersizes.hpp>
#include <blockynumber.hpp>
#include <bitwriter.hpp>
#include <bitreader.hpp>
#include <blockymetadata.hpp>
#include <methods/methods.hpp>

#include <block.hpp>

class DecompressionMethod {
protected:
	BlockyMetadata const& metadata;
	CompressionMethod** methods;

	DecompressionMethod
	(
		BlockyMetadata const& metadata
	)
		: metadata(metadata) { }

public:
	virtual int read
	(
		BitReader& reader,
		BitWriter& writer,
		Block block
	) = 0;

	static Block read_default_block_header
	(
		BitReader const& reader,
		BlockyMetadata const& metadata
	);

	static BlockyNumber read_single_value_without_control_bit
	(
		BitReader const& reader,
		BlockyMetadata const& metadata
	);
};


#endif /* end of include guard: DECOMPRESSIONMETHOD_HPP */
