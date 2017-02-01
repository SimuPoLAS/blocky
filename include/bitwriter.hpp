#ifndef BITWRITER_HPP
#define BITWRITER_HPP

#include <fstream>

class BitWriter {
private:
    FILE* file;
    // TODO: try use it as reference and notas pointer
    uint8_t* buffer;
    uint8_t offset;

public:
    BitWriter(FILE* file)
        : file(file) { }

    ~BitWriter();

    void write(uint64_t data, uint8_t count);
    void write(uint8_t data, uint8_t count);
    void flush();
};

#endif /* end of include guard: BITWRITER_HPP */
