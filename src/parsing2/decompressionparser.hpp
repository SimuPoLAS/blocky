#ifndef DECOMPRESSIONPARSER_HPP
#define DECOMPRESSIONPARSER_HPP

#include <memory>
#include <vector>

#include "../compressedsection.hpp"
#include "../lzmaio.hpp"

class DecompressionParser
{
private:
public:
    DecompressionParser();

    int fill_buffer(char* buffer, int bufferSize, std::vector<CompressedSection>& sections);
};

#endif /* end of include guard: DECOMPRESSIONPARSER_HPP */
