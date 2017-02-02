#ifndef BLOCKYCOMPRESSION_HPP
#define BLOCKYCOMPRESSION_HPP

#include <vector>
#include <memory>

#include <bitwriter.hpp>
#include <blockynumber.hpp>
#include <block.hpp>
#include <blockymetadata.hpp>
#include <blockfinding/blockfinding.hpp>

using namespace std;

class BlockyCompression
{
private:
    BitWriter writer;
    unique_ptr<Blockfinding> blockfinding;
    int32_t totalPostCompressionOptimisationBlockValues;

public:
    vector<BlockyNumber> Values;
    vector<Block> Blocks;
    BlockyMetadata Metadata;

    BlockyCompression(FILE* file);

    void report(BlockyNumber& number);
    void report(BlockyNumber* numbers, size_t offset, size_t count);
    void finish();
    void post_compression_optimisation();
    void write();
};

#endif /* end of include guard: BLOCKYCOMPRESSION_HPP */
