#ifndef BLOCKYDECOMPRESSION_HPP
#define BLOCKYDECOMPRESSION_HPP

#include <vector>

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
	BitReader reader;
	BitWriter writer;
	BlockyMetadata metadata;
	std::vector<DecompressionMethod> methods;

	// TODO: make a constructor

	// TODO: get this crap to work
	DecompressionMethod get_method_for_block(Block block);
public:
	void decompress();
	void write(BlockyNumber value);
};

#endif /* end of include guard: BLOCKYDECOMPRESSION_HPP */