#ifndef FLOATSIMILARCOMPRESSION_HPP
#define FLOATSIMILARCOMPRESSION_HPP

#include <methods/compressionmethod.hpp>

class FloatSimilarCompression
    : public CompressionMethod
{
private:

public:
    FloatSimilarCompression
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

#endif /* end of include guard: FLOATSIMILARCOMPRESSION_HPP */
