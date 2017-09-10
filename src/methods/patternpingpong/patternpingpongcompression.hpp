#ifndef PATTERNPINGPONGCOMPRESSION_HPP
#define PATTERNPINGPONGCOMPRESSION_HPP

#include <vector>

#include "../compressionmethod.hpp"

using namespace std;

// TODO: decide whether this class should continue to inherit from
// CompressionMethod, since it breaks some usage rules:
// - Not having a real process_value()
// - using and writing intern variables in write()
// - only used in BlockyCompression in post_compression_optimisation()

// NOTE: when removing this method from initializedCompressionMethods
// in Blockfinding by making it not inherit from CompressionMethod
// and using it only in BlockyCompression, we can make
// the types const again => performance boost

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

    vector<PatternPingPongMetadata> PingPongPatternLengths;

    PatternPingPongCompression
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

#endif /* end of include guard: PATTERNPINGPONGCOMPRESSION_HPP */
