#ifndef BLOCKYALGORITHM_HPP
#define BLOCKYALGORITHM_HPP

#include <memory>

#include "lzmaio.hpp"
#include "reporter.hpp"
#include "blockycompression.hpp"
#include "blockydecompression.hpp"
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

	void decompress(FILE* file, size_t width = 1) {
		if (width == 0)
		{
			BlockyDecompression(file).decompress();
			return;
		}

		std::vector<shared_ptr<BlockyDecompression>> decomp;
		decomp.resize(width);

		// the norbi way (using good code)
		for (size_t i = 0; i < width; i++)
		{
			decomp[i] = make_shared<BlockyDecompression>(file);
			decomp[i]->initialize(decomp[i]->metadata.ValueCount);
			decomp[i]->decompress();
		}

		auto len = decomp[0]->values.size();

		for (size_t i = 0; i < len; i++)
			for (size_t j = 0; j < decomp.size(); j++)
				decomp[0]->report(decomp[j]->values[i]);
	}
};

#endif /* end of include guard: BLOCKYALGORITHM_HPP */
