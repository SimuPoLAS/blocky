#ifndef DECOMPSTREAMBASE_HPP
#define DECOMPSTREAMBASE_HPP

#include <iostream>
#include <fstream>

#include "decompstreambuffer.hpp"

class DecompStreamBase
    : virtual public std::ios
{
protected:
    DecompStreamBuffer buffer;

public:
    DecompStreamBase();
    DecompStreamBase(const char* name, int open_mode);
    ~DecompStreamBase();

    DecompStreamBuffer* rdbuf()
    {
        std::cout << "rdbuf" << '\n';
        return &buffer;
    }

    void open(const char* name, int open_mode);
    void close();
};

#endif /* end of include guard: DECOMPSTREAMBASE_HPP */
