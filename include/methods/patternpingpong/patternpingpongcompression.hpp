#ifndef PATTERNPINGPONGCOMPRESSION_HPP
#define PATTERNPINGPONGCOMPRESSION_HPP

#include <vector>

#include <methods/compressionmethod.hpp>

using namespace std;

class PatternPingPongCompression
    : public CompressionMethod
{
private:
    int32_t probabilityMetadataIndex;

public:
    struct PatternPingPongMetadata
    {
        int32_t BlockIndex;
        uint8_t Length;
        PatternPingPongMetadata(uint8_t length, int32_t blockIndex)
            : Length(length)
            , BlockIndex(blockIndex) { }
        PatternPingPongMetadata(PatternPingPongMetadata const& other)
            : Length(other.Length)
            , BlockIndex(other.BlockIndex) { }
    };

    vector<PatternPingPongMetadata> const PingPongPatternLengths;

    PatternPingPongCompression
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

#endif /* end of include guard: PATTERNPINGPONGCOMPRESSION_HPP */
