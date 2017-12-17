#include "decompstreambase.hpp"

DecompStreamBase::DecompStreamBase()
{
    init(&buffer);
}

DecompStreamBase::DecompStreamBase(const char* name, int open_mode)
{
    init(&buffer);
    open(name, open_mode);
}

DecompStreamBase::~DecompStreamBase()
{
    close();
}

void DecompStreamBase::open(const char* name, int open_mode)
{
    if (!buffer.open(name, open_mode))
        setstate(std::ios::badbit);
}

void DecompStreamBase::close()
{
    if (buffer.is_open())
        if (!buffer.close())
            setstate(std::ios::badbit);
}
