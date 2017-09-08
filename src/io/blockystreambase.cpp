#include "blockystreambase.hpp"

BlockyStreamBase::BlockyStreamBase()
{
    init(&buffer);
}

BlockyStreamBase::BlockyStreamBase(const char* name, int open_mode)
{
    init(&buffer);
    open(name, open_mode);
}

BlockyStreamBase::~BlockyStreamBase()
{
    close();
}

void BlockyStreamBase::open(const char* name, int open_mode)
{
    if (!buffer.open(name, open_mode))
        setstate(std::ios::badbit);
}

void BlockyStreamBase::close()
{
    if (buffer.is_open())
        if (!buffer.close())
            setstate(std::ios::badbit);
}
