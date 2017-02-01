#ifndef BITREADER_HPP
#define BITREADER_HPP

#include <fstream>

class BitReader
{
private:
    FILE* file;
    uint8_t buffer;
    uint8_t offset;

    uint8_t read_unaligned_byte();

public:
    BitReader(FILE* file);

    uint64_t read(uint8_t count);
    uint8_t read_byte(uint8_t count);
};

#endif /* end of include guard: BITREADER_HPP */
