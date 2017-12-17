#ifndef DECOMPSTREAMBUFFER_HPP
#define DECOMPSTREAMBUFFER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "../lzmaio.hpp"
#include "../parsing2/decompressionparser.hpp"

using namespace std;

class DecompStreamBuffer
    : public std::streambuf
{
private:
    static const int bufferSize = 4096;

    char buffer[bufferSize];

    LZMAFILE* meta;
    LZMAFILE* data;

    bool opened;
    int mode;
    bool last = false;

    unique_ptr<DecompressionParser> decompression;

    int flush_buffer();

public:
    DecompStreamBuffer();
    ~DecompStreamBuffer();

    bool is_open() { return opened; }

    DecompStreamBuffer* open(const char* name, int open_mode);
    DecompStreamBuffer* close();

    int overflow(int c = EOF) override;
    int underflow() override;
    int sync() override;
};

#endif /* end of include guard: DECOMPSTREAMBUFFER_HPP */
