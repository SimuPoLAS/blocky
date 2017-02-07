#ifndef FLOATSIMMILARCOMPRESSION_HPP
#define FLOATSIMMILARCOMPRESSION_HPP

#include <methods/compressionmethod.hpp>

class FloatSimmilarCompression
    : public CompressionMethod
{
private:

public:
    FloatSimmilarCompression
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
        int32_t& valueIndex
    ) override;
};

#endif /* end of include guard: FLOATSIMMILARCOMPRESSION_HPP */
