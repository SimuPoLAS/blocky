#ifndef BLOCKYALGORITHM_HPP
#define BLOCKYALGORITHM_HPP

#include <memory>

#include "reporter.hpp"
#include "blockycompression.hpp"
#include "compressionsplitter.hpp"

using namespace std;

class BlockyAlgorithm
{
public:
    shared_ptr<Reporter> compress(FILE* file, size_t width, int elements)
    {
        if (width == 1)
            return shared_ptr<Reporter>(new BlockyCompression(file));

        auto compressions = shared_ptr<shared_ptr<Reporter>>
        (
            new shared_ptr<Reporter>[width]
        );
        for (size_t i = 0; i < width; i++)
            compressions.get()[i] = shared_ptr<BlockyCompression>
            (
                new BlockyCompression(file)
            );
        return shared_ptr<CompressionSplitter>
        (
            new CompressionSplitter(compressions, width)
        );
    }
};

#endif /* end of include guard: BLOCKYALGORITHM_HPP */
