#ifndef BLOCKYDECOMPRESSION_HPP
#define BLOCKYDECOMPRESSION_HPP

#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>

#include "bitreader.hpp"
#include "blockymetadata.hpp"
#include "block.hpp"
#include "blockynumber.hpp"
#include "blockynumbersaver.hpp"
#include "methods/methods.hpp"
#include "methods/decompressionmethod.hpp"
#include "reporter.hpp"

class BlockyDecompression
{
private:
    LZMAFILE* data;
    size_t index = 0;
    size_t size;
    BitReader& reader;
    DecompressionMethod* methods[METHODS_COUNT];
    BlockyNumberSaver& saver;

    DecompressionMethod* get_method_for_block(Block block);
public:
    BlockyMetadata metadata;

    BlockyDecompression(LZMAFILE* data, BlockyNumberSaver& saver, BitReader& reader, size_t size);
    ~BlockyDecompression();

    void decompress();
    void write(BlockyNumber value);
};

#endif /* end of include guard: BLOCKYDECOMPRESSION_HPP */