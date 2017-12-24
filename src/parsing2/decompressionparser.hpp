#ifndef DECOMPRESSIONPARSER_HPP
#define DECOMPRESSIONPARSER_HPP

#include <memory>
#include <vector>

#include "../compressedsection.hpp"
#include "../lzmaio.hpp"

class DecompressionParser
{
private:
    LZMAFILE* data;
    LZMAFILE* meta;

    size_t total;
    size_t current;
    size_t to_read;

    bool ended;
    bool decompress;
public:
    DecompressionParser(LZMAFILE* meta, LZMAFILE* data);

    int fill_buffer(char* buffer, int bufferSize, std::vector<CompressedSection>& sections);
};

#endif /* end of include guard: DECOMPRESSIONPARSER_HPP */
