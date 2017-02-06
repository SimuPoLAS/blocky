#ifndef PATTERNOFFSETCOMPRESSION_HPP
#define PATTERNOFFSETCOMPRESSION_HPP

#include <methods/compressionmethod.hpp>

class PatternOffsetCompression
    : public CompressionMethod
{
private:
    /* data */

public:
    PatternOffsetCompression
    (
        vector<shared_ptr<BlockyNumber>> const& values,
        BlockyMetadata const& metadata,
        HeaderSizes const& headers,
        CompressionMethod** methods
    )
        : CompressionMethod(values, metadata, headers, methods) { }

    bool virtual process_value
    (
        Block& block,
        shared_ptr<const BlockyNumber> value,
        int32_t index,
        int32_t& bitDiff
    ) override;

    void virtual write
    (
        BitWriter writer,
        Block block,
        int32_t& valueIndex
    ) override;
};

#endif /* end of include guard: PATTERNOFFSETCOMPRESSION_HPP */
