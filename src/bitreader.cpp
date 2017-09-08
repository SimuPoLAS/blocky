#include <math.h>

#include "bitreader.hpp"

BitReader::BitReader(FILE* file)
    : file(file)
	, buffer(0)
	, offset(0)
{
    auto c = getc(file);
    if (c == EOF)
        // TODO: Throw a specified for this case value (not just 0)
        throw 0;
    buffer = c;
}

uint8_t BitReader::read_unaligned_byte()
{
    auto c = getc(file);
    if (c == EOF)
        // TODO: Throw a specified for this case value (not just 0)
        throw 0;
    return uint8_t(c);
}

uint64_t BitReader::read(uint8_t count)
{
    if (offset == 0)
    {
        buffer = read_unaligned_byte();
        offset = 0;
    }
    uint64_t data = 0;
    auto _offset = 0;
    do
    {
        auto bitsLeft = 8 - offset;
        if (bitsLeft >= count)
        {
            data |= (buffer & uint64_t(pow(2, count) - 1)) << offset;
            offset += count;
            buffer = uint8_t(buffer >> count);
            return data;
        }
        data |= uint64_t(buffer << offset);
        count -= uint8_t(bitsLeft);
        _offset += bitsLeft;
        offset = 0;
        buffer = read_unaligned_byte();
    } while(count > 0);
    return data;
}

uint8_t BitReader::read_byte(uint8_t count)
{
    if (offset == 8 && count == 8)
        return read_unaligned_byte();
    return uint8_t(read(count));
}
