#ifndef PATTERNOFFSETCOMPRESSION_HPP
#define PATTERNOFFSETCOMPRESSION_HPP

#include "../compressionmethod.hpp"

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
        const BlockyNumber& value,
        int32_t index,
        int32_t& bitDiff
    ) override;

    void virtual write
    (
        BitWriter& writer,
        Block block,
        size_t& valueIndex
    ) override;
};

#endif /* end of include guard: PATTERNOFFSETCOMPRESSION_HPP */
