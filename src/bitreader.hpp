#ifndef BITREADER_HPP
#define BITREADER_HPP

#include <fstream>

#include "lzmaio.hpp"

class BitReader
{
private:
    LZMAFILE* file;
    uint8_t buffer;
    uint8_t offset;

    uint8_t read_unaligned_byte();

public:
    BitReader(LZMAFILE* file);

    uint64_t read(uint8_t count);
    uint8_t read_byte(uint8_t count);
};

#endif /* end of include guard: BITREADER_HPP */
