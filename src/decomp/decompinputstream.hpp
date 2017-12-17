#ifndef DECOMPINPUTSTREAM_HPP
#define DECOMPINPUTSTREAM_HPP

#include <iostream>
#include <fstream>

#include "decompstreambase.hpp"

class DecompInputStream
    : public std::istream
    , public DecompStreamBase
{
public:
    DecompInputStream()
        : std::istream(&buffer) {}

    DecompInputStream(const char* name, int open_mode = std::ios::in)
        : std::istream(&buffer)
        , DecompStreamBase(name, open_mode) {}

    DecompStreamBuffer* rdbuf()
    {
        std::cout << "rdbuf (in)" << '\n';
        return DecompStreamBase::rdbuf();
    }

    void open(const char* name, int open_mode = std::ios::in)
    {
        DecompStreamBase::open(name, open_mode);
    }
};

#endif /* end of include guard: DECOMPINPUTSTREAM_HPP */
