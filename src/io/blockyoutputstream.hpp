#ifndef BLOCKYOUTPUTSTREAM_HPP
#define BLOCKYOUTPUTSTREAM_HPP

#include <iostream>
#include <fstream>

#include <io/blockystreambase.hpp>

class BlockyOutputStream
    : public std::ostream
    , public BlockyStreamBase
{
public:
    BlockyOutputStream()
        : std::ostream(&buffer) {}

    BlockyOutputStream(const char* name, int open_mode = std::ios::out)
        : BlockyStreamBase(name, open_mode)
        , std::ostream(&buffer) {}

    BlockyStreamBuffer* rdbuf()
    {
        std::cout << "rdbuf (out)" << '\n';
        return BlockyStreamBase::rdbuf();
    }
    void open(const char* name, int open_mode = std::ios::out)
    {
        BlockyStreamBase::open(name, open_mode);
    }
};

#endif /* end of include guard: BLOCKYOUTPUTSTREAM_HPP */
