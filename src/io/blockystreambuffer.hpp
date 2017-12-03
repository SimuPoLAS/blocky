#ifndef BLOCKYSTREAMBUFFER_HPP
#define BLOCKYSTREAMBUFFER_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "../lzmaio.hpp"
#include "../parsing/mainparser.hpp"

using namespace std;

class BlockyStreamBuffer
    : public std::streambuf
{
private:
    static const int bufferSize = 840;

    char buffer[bufferSize];

    LZMAFILE* data;
	LZMAFILE* meta;

    bool opened;
    int mode;
	bool last = false;

    unique_ptr<MainParser> parser;

    // uint32_t width;
    // uint32_t position;
    // uint32_t processed;
    //
    // uint32_t tokenStartPosition;

    int flush_buffer();

    // bool needs(int amount);
    // int skip(int amount);
    // int skip_until(char const escape);
    // int skip_until(char const* escapes, int size);
    // int eat(char& c);
    // int eat_until(char const escape, string& s);
    // int eat_until(char const* escapes, int size, string& s);
    //
    // Token next_token();

public:
    BlockyStreamBuffer();
    ~BlockyStreamBuffer();

    bool is_open() { return opened; }

    BlockyStreamBuffer* open(const char* name, int open_mode);
    BlockyStreamBuffer* close();

    int overflow(int c = EOF) override;
    int underflow() override;
    int sync() override;
};

#endif /* end of include guard: BLOCKYSTREAMBUFFER_HPP */
