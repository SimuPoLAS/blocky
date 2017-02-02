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
        vector<BlockyNumber> const& values,
        BlockyMetadata const& metadata,
        HeaderSizes const& headers,
        CompressionMethod const** methods
    )
        : CompressionMethod(values, metadata, headers, methods) { }

    bool virtual process_value
    (
        Block& block,
        BlockyNumber const& value,
        int32_t index,
        int32_t& bitDiff
    ) const override;

    void virtual write
    (
        BitWriter writer,
        Block block,
        int32_t& valueIndex
    ) const override;
};

#endif /* end of include guard: FLOATSIMMILARCOMPRESSION_HPP */
