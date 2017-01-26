#ifndef BLOCKYINPUTSTREAM_HPP
#define BLOCKYINPUTSTREAM_HPP

#include <iostream>
#include <fstream>

#include <blockystreambase.hpp>

class BlockyInputStream
    : public std::istream
    , public BlockyStreamBase
{
public:
    BlockyInputStream()
        : std:istream(&buf) {}

    BlockyInputStream(const char* name, int open_mode = std::ios::in)
        : std::istream(&buf)
        , BlockyStreamBase(name, open_mode) {}
};

#endif /* end of include guard: BLOCKYINPUTSTREAM_HPP */
