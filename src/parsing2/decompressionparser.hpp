#ifndef DECOMPRESSIONPARSER_HPP
#define DECOMPRESSIONPARSER_HPP

#include <memory>

#include "compressedsectionparser.hpp"
#include "../lzmaio.hpp"

class DecompressionParser
{
private:
    size_t total_parsed = 0;
    LZMAFILE* data;
    LZMAFILE* meta;
    std::vector<std::shared_ptr<Decompression>> parsers;

    std::shared_ptr<Decompression> curr_parser = nullptr;
public:
    DecompressionParser(LZMAFILE* data, LZMAFILE* meta);

    int parse
    (
        const char* buffer,
        int offset,
        int count,
        bool last
    );

    void end();
};

#endif /* end of include guard: DECOMPRESSIONPARSER_HPP */
