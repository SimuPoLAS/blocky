#include <vector>

#include <blockfinding/blockfinding.hpp>

#include <methods/floatsimmilar/floatsimmilarcompression.hpp>
#include <methods/numbersnoexp/numbersnoexpcompression.hpp>
#include <methods/patternoffset/patternoffsetcompression.hpp>
#include <methods/patternpingpong/patternpingpongcompression.hpp>
#include <methods/patternsame/patternsamecompression.hpp>

Blockfinding::Blockfinding
(
    vector<BlockyNumber> const& values,
    BlockyMetadata const& metadata
)
    : Values(values)
    , ValueCount(values.size())
    , Metadata(metadata)
    , Headers(metadata)
    , patternPredictor(values)
{
    initializedCompressionMethods[(int)Methods::PatternSame] =
        new PatternSameCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
    initializedCompressionMethods[(int)Methods::FloatSimmilar] =
        new FloatSimmilarCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
    initializedCompressionMethods[(int)Methods::NumbersNoExp] =
        new NummbersNoExpCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
    initializedCompressionMethods[(int)Methods::PatternOffset] =
        new PatternOffsetCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
    initializedCompressionMethods[(int)Methods::PatternPingPong] =
        new PatternPingPongCompression
        (
            values,
            metadata,
            Headers,
            initializedCompressionMethods
        );
}
