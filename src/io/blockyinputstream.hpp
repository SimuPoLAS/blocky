#ifndef BLOCKYINPUTSTREAM_HPP
#define BLOCKYINPUTSTREAM_HPP

#include <iostream>
#include <fstream>

#include "blockystreambase.hpp"

class BlockyInputStream
    : public std::istream
    , public BlockyStreamBase
{
public:
    BlockyInputStream()
        : std::istream(&buffer) {}

    BlockyInputStream(const char* name, int open_mode = std::ios::in)
        : std::istream(&buffer)
        , BlockyStreamBase(name, open_mode) {}

	BlockyStreamBuffer* rdbuf()
	{
		std::cout << "rdbuf (in)" << '\n';
		return BlockyStreamBase::rdbuf();
	}

	void open(const char* name, int open_mode = std::ios::in)
	{
		BlockyStreamBase::open(name, open_mode);
	}
};

#endif /* end of include guard: BLOCKYINPUTSTREAM_HPP */
