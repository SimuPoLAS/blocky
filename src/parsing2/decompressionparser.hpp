#ifndef DECOMPRESSIONPARSER_HPP
#define DECOMPRESSIONPARSER_HPP

#include <memory>
#include <vector>

#include "../compressedsection.hpp"
#include "../blockynumbersaver.hpp"
#include "../blockyalgorithm.hpp"
#include "../lzmaio.hpp"

class DecompressionParser
{
private:
    LZMAFILE* data;
    LZMAFILE* meta;

    size_t total;
    size_t current_meta;
    size_t current_data;
    size_t to_read;

    int buf_size;

    bool ended;
    bool decompress;

    std::vector<CompressedSection>& sections;
    std::vector<char> data_buffer;
    BlockyNumberSaver current_numbersaver;
    BlockyAlgorithm algorithm;
    BitReader reader;
public:
    DecompressionParser(LZMAFILE* meta, LZMAFILE* data, std::vector<CompressedSection>& sections);

    int fill_buffer(char* buffer, int buffer_size);
};

#endif /* end of include guard: DECOMPRESSIONPARSER_HPP */
