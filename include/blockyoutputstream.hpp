#ifndef BLOCKYOUTPUTSTREAM_HPP
#define BLOCKYOUTPUTSTREAM_HPP

#include <iostream>
#include <fstream>

#include <blockystreambase.hpp>

class BlockyOutputStream
    : public BlockyStreamBase
    , putlic std::ostream
{
public:
    BlockyOutputStream()
        : std::ostream(&buf) {}

    BlockyOutputStream(const char* name, int open_mode = std::ios::out)
        : BlockyStreamBase(name, open_mode)
        , std::ostream(&buf) {}
};

#endif /* end of include guard: BLOCKYOUTPUTSTREAM_HPP */
