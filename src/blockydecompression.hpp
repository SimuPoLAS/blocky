#ifndef BLOCKYDECOMPRESSION_HPP
#define BLOCKYDECOMPRESSION_HPP

#include <vector>
#include <memory>

#include "bitreader.hpp"
#include "blockymetadata.hpp"
#include "block.hpp"
#include "blockynumber.hpp"
#include "methods/methods.hpp"
#include "methods/decompressionmethod.hpp"
#include "reporter.hpp"

class BlockyDecompression
	: public Reporter
{
private:
	size_t index = 0;
	BitReader reader;
	BlockyMetadata metadata;
	DecompressionMethod* methods[METHODS_COUNT];

	// marerreporter replacement members
	std::vector<std::string> buffer;
	size_t reporter_position;
	size_t reporter_get_size();
	void reporter_set_size(size_t size);

	std::unique_ptr<DecompressionMethod> get_method_for_block(Block block);
public:
	std::vector<BlockyNumber> values;

	BlockyDecompression(FILE* file);

	void initialize(int value_count);
	void decompress();
	void write(BlockyNumber value);

	// marerreporter replacement members
	void report(BlockyNumber value) override;
	void report(BlockyNumber* values, size_t offset, size_t amount) override;
	void finish() override;
};

#endif /* end of include guard: BLOCKYDECOMPRESSION_HPP */