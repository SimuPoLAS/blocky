#ifndef BITWRITER_HPP
#define BITWRITER_HPP

#include <fstream>
#include <iostream>
#include <memory>

#include "lzmaio.hpp"

using namespace std;

class BitWriter {
private:
	LZMAFILE* file;
    // TODO: try use it as reference and notas pointer
    uint8_t buffer;
    uint8_t offset;

public:
	// TODO: decide whether we keep position or not
	uint64_t Position;

    BitWriter(LZMAFILE* file)
        : file(file)
		, buffer(0)
		, offset(0)
		, Position(0) { }

    ~BitWriter();

    void write(uint64_t data, uint8_t count);
    void write_byte(uint8_t data, uint8_t count);
    void flush();
};

#endif /* end of include guard: BITWRITER_HPP */
