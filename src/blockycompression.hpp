#ifndef BLOCKYCOMPRESSION_HPP
#define BLOCKYCOMPRESSION_HPP

#include <vector>
#include <memory>

#include "bitwriter.hpp"
#include "blockynumber.hpp"
#include "block.hpp"
#include "blockymetadata.hpp"
#include "blockfinding/blockfinding.hpp"
#include "reporter.hpp"

using namespace std;

class BlockyCompression : public Reporter
{
private:
    BitWriter writer;
    unique_ptr<Blockfinding> blockfinding;
    int32_t totalPostCompressionOptimisationBlockValues;

public:
    vector<shared_ptr<BlockyNumber>> Values;
    vector<Block> Blocks;
    BlockyMetadata Metadata;

    // TODO: decide whether use normal ptr or shared_ptr for file
    BlockyCompression(FILE* file);

    void report(BlockyNumber number) override;
    void report
    (
        // TODO: decide whether to use normal pointers
        // as parameter or shared_ptr
        BlockyNumber* numbers,
        size_t offset,
        size_t count
    ) override;
    void finish() override;
    void post_compression_optimisation();
    void write();
};

#endif /* end of include guard: BLOCKYCOMPRESSION_HPP */
