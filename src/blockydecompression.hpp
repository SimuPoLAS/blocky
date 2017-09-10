#ifndef BLOCKYDECOMPRESSION_HPP
#define BLOCKYDECOMPRESSION_HPP

#include <vector>
#include <memory>

#include "bitreader.hpp"
#include "reporter.hpp"
#include "blockymetadata.hpp"
#include "block.hpp"
#include "blockynumber.hpp"
#include "methods/methods.hpp"
#include "methods/decompressionmethod.hpp"
#include "reporter.hpp"

class BlockyDecompression
{
private:
	size_t index = 0;
	BitReader reader;
	BitWriter& writer;
	BlockyMetadata metadata;
	DecompressionMethod* methods[METHODS_COUNT];

	std::unique_ptr<DecompressionMethod> get_method_for_block(Block block);
public:
	BlockyNumber* values;

	BlockyDecompression(FILE* reader);

	void initialize(int value_count);
	void decompress();
	void write(BlockyNumber value);
};

#endif /* end of include guard: BLOCKYDECOMPRESSION_HPP */