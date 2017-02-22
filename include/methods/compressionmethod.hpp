#ifndef COMPRESSIONMETHOD_HPP
#define COMPRESSIONMETHOD_HPP

#include <vector>

#include <blockfinding/headersizes.hpp>
#include <blockynumber.hpp>
#include <bitwriter.hpp>
#include <blockymetadata.hpp>
#include <methods/methods.hpp>

#include <block.hpp>

class CompressionMethod {
protected:
    vector<shared_ptr<BlockyNumber>> const& values;
    BlockyMetadata const& metadata;
    // length equals size of methods enum
    CompressionMethod** methods;
    HeaderSizes const& headers;

    void write_default_blockheader
    (
        BitWriter& writer,
        Block block
    ) const;
    void write_single_value_without_controlbit
    (
        BitWriter& writer,
        const BlockyNumber& value
    ) const;

public:
    CompressionMethod
    (
        vector<shared_ptr<BlockyNumber>> const& values,
        BlockyMetadata const& metadata,
        HeaderSizes const& headers,
        CompressionMethod** methods
    )
        : values(values)
        , metadata(metadata)
        , headers(headers)
        , methods(methods) { }

    bool virtual process_value
    (
        Block& block,
        const BlockyNumber& value,
        int32_t index,
        int32_t& bitDiff
    ) = 0;

    void virtual write
    (
        BitWriter& writer,
        Block block,
        int32_t& valueIndex
    ) = 0;
};

#endif /* end of include guard: COMPRESSIONMETHOD_HPP */
