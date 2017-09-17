#ifndef BLOCKYALGORITHM_HPP
#define BLOCKYALGORITHM_HPP

#include <memory>

#include "reporter.hpp"
#include "blockycompression.hpp"
#include "blockydecompression.hpp"
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

	void decompress(FILE* file, size_t width = 1) {
		if (width == 0)
		{
			BlockyDecompression(file).decompress();
			return;
		}

		std::vector<BlockyDecompression> decomp;
		decomp.resize(width);

		// the norbi way (using good code)
		for (size_t i = 0; i < width; i++)
		{
			decomp[i] = BlockyDecompression(file);
			decomp[i].initialize(decomp[i].metadata.ValueCount);
			decomp[i].decompress();
		}

		auto len = decomp[0].values.size();

		for (size_t i = 0; i < len; i++)
			for (size_t j = 0; j < decomp.size(); j++)
				decomp[0].report(decomp[j].values[i]);
	}
};

#endif /* end of include guard: BLOCKYALGORITHM_HPP */
