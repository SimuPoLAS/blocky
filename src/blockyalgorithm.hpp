#ifndef BLOCKYALGORITHM_HPP
#define BLOCKYALGORITHM_HPP

#include <memory>

#include "lzmaio.hpp"
#include "reporter.hpp"
#include "blockycompression.hpp"
#include "blockydecompression.hpp"
#include "blockynumber.hpp"
#include "blockynumbersaver.hpp"
#include "compressionsplitter.hpp"

using namespace std;

class BlockyAlgorithm
{
public:
    shared_ptr<Reporter> compress(LZMAFILE* file, size_t width, int elements)
    {
        if (width == 1)
            return make_shared<BlockyCompression>(file);

        auto compressions = std::vector<shared_ptr<Reporter>>(width);
        for (size_t i = 0; i < width; i++)
            compressions[i] = make_shared<BlockyCompression>(file);
        return make_shared<CompressionSplitter>(compressions, width);
    }

    BlockyNumberSaver decompress(LZMAFILE* data, char* buffer, BitReader& reader, size_t size  = 0) {
        /*
        auto len = decomp[0]->values.size();

        for (size_t i = 0; i < len; i++)
            for (size_t j = 0; j < decomp.size(); j++)
                decomp[0]->report(decomp[j]->values[i]);
        */

        printf("entered BlockyAlgorithm\n");

        // handle case when compressed section has a size of 0
        //if (size == 0) { }

        //BlockyNumberSaver decomp[size]
        BlockyNumberSaver decomp;

        /*
        for (size_t i = 0; i < size; i++) {
            auto blockyDecomp = new BlockyDecompression(data, buffer, decomp[i], reader);
            decomp[i].initialize(blockyDecomp->metadata.ValueCount);
            // blockyDecomp->decompress();
            printf("metadata.ValueCount for this section is %d", blockyDecomp->metadata.ValueCount);
        }*/
        // TODO: figure out how to handle multidimensional data
        // (vectors and tensors have a size of 3 and 9 respectively)
        auto blockyDecomp = new BlockyDecompression(data, decomp, reader, size);
        decomp.initialize(blockyDecomp->metadata.ValueCount);
        blockyDecomp->decompress();
        printf("metadata.ValueCount for this section is %d\n", blockyDecomp->metadata.ValueCount);

        return decomp;
    }
};

#endif /* end of include guard: BLOCKYALGORITHM_HPP */
