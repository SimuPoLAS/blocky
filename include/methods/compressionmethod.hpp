#ifndef COMPRESSIONMETHOD_HPP
#define COMPRESSIONMETHOD_HPP

#include <vector>

#include <blockfinding/headersizes.hpp>
#include <blockynumber.hpp>
#include <bitwriter.hpp>
#include <blockymetadata.hpp>
#include <methods/methods.hpp>

struct Block;

class CompressionMethod {
protected:
    vector<BlockyNumber> const& values;
    BlockyMetadata const& metadata;
    // length equals size of methods enum
    CompressionMethod const** methods;
    size_t methodsCount;
    HeaderSizes const& headers;

    void write_default_blockheader(BitWriter writer, Block block);
    void write_single_value_without_controlbit
    (
        BitWriter writer,
        BlockyNumber value
    );

public:
    CompressionMethod
    (
        vector<BlockyNumber> const& values,
        BlockyMetadata const& metadata,
        HeaderSizes const& headers,
        CompressionMethod const** methods,
        size_t methodsCount
    )
        : values(values)
        , metadata(metadata)
        , headers(headers)
        , methods(methods)
        , methodsCount(methodsCount) { }

    bool virtual process_value
    (
        Block& block,
        BlockyNumber value,
        int32_t index,
        int32_t& bitDiff
    ) = 0;

    void virtual write(BitWriter writer, Block block, int32_t& valueIndex) = 0;
};

#endif /* end of include guard: COMPRESSIONMETHOD_HPP */
