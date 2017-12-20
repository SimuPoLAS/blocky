#ifndef DECOMPSTREAMBUFFER_HPP
#define DECOMPSTREAMBUFFER_HPP

#include <limits.h>

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>

#include "../lzmaio.hpp"
#include "../parsing2/decompressionparser.hpp"
#include "../compressedsection.hpp"

#define OK 1
#define FINISHED 0
#define NOT_OK -1
#define BUFFER_SHORT -2

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
    int meta_processed;
    int data_processed;

    unique_ptr<DecompressionParser> decompression;

    //int flush_buffer();

public:
    DecompStreamBuffer();
    ~DecompStreamBuffer();

    bool is_open() { return opened; }

    DecompStreamBuffer* open(const char* name, int open_mode);
    DecompStreamBuffer* close();

    //int overflow(int c = EOF) override;
    int underflow() override;
    //int sync() override;
};

#endif /* end of include guard: DECOMPSTREAMBUFFER_HPP */
