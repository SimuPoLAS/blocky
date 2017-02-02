#ifndef NUMMBERSNOEXPCOMPRESSION_HPP
#define NUMMBERSNOEXPCOMPRESSION_HPP

#include <methods/compressionmethod.hpp>

class NummbersNoExpCompression
    : public CompressionMethod
{
private:
    /* data */

public:
    NummbersNoExpCompression
    (
        vector<BlockyNumber> const& values,
        BlockyMetadata const& metadata,
        HeaderSizes const& headers,
        CompressionMethod** methods
    )
        : CompressionMethod(values, metadata, headers, methods) { }

    bool virtual process_value
    (
        Block& block,
        BlockyNumber const& value,
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

#endif /* end of include guard: NUMMBERSNOEXPCOMPRESSION_HPP */
