#include <math.h>
#include <iostream>

#include <bitwriter.hpp>

BitWriter::~BitWriter()
{
    flush();
}

void BitWriter::write(uint64_t data, uint8_t count)
{
    std::cout << "BITWRITER" << '\n';
    std::cout << "data: " << data << '\n';
    std::cout << "count: " << to_string(count) << '\n';
    do
    {
        auto bitsLeft = 8 - offset;
        if (bitsLeft > count)
        {
            buffer |= uint8_t((data & uint64_t(pow(2, count) - 1)) << offset);
            offset += count;
            return;
        }
        buffer |= uint8_t((data & uint64_t(pow(2, bitsLeft) - 1)) << offset);
        std::cout << "bitwriter is writing!~" << '\n';
        fwrite(&buffer, 1, 1, file);
        offset = 0;
        buffer = 0;
        count -= (uint8_t)bitsLeft;
        data = data >> bitsLeft;
    } while (count > 0);
}

void BitWriter::write_byte(uint8_t data, uint8_t count)
{
    write(data, count);
}

void BitWriter::flush()
{
    if (buffer == 0 && offset == 0)
        return;
    fwrite(&buffer, 1, 1, file);
    buffer = 0;
    offset = 0;
}
