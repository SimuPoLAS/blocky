#ifndef BLOCKYSTREAMBASE_HPP
#define BLOCKYSTREAMBASE_HPP

#include <iostream>
#include <fstream>

#include <blockystreambuffer.hpp>

class BlockyStreamBase
    : virtual public std::ios
{
private:
    BlockyStreamBuffer buffer;

public:
    BlockyStreamBase();
    BlockyStreamBase(const char* name, int open_mode);
    ~BlockyStreamBase();

    BlockyStreamBuffer* rdbuf() { return buffer; }

    void open(const char* name, int open_mode);
    void close();
};

#endif /* end of include guard: BLOCKYSTREAMBASE_HPP */
