#ifndef BLOCKYDECOMPRESSION_HPP
#define BLOCKYDECOMPRESSION_HPP

#include <vector>
#include <memory>

#include "bitreader.hpp"
#include "bitwriter.hpp"
#include "blockymetadata.hpp"
#include "block.hpp"
#include "blockynumber.hpp"
#include "methods/methods.hpp"
#include "methods/decompressionmethod.hpp"
#include "reporter.hpp"

class BlockyDecompression
{
private:
	BitReader& reader;
	BitWriter& writer;
	BlockyMetadata metadata;
	DecompressionMethod** methods;

	std::unique_ptr<DecompressionMethod> get_method_for_block(Block block);
public:
	void decompress();
	void write(BlockyNumber value);
};

#endif /* end of include guard: BLOCKYDECOMPRESSION_HPP */