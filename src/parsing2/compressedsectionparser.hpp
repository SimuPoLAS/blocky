#ifndef COMPRESSEDSECTION_PARSER_HPP
#define COMPRESSEDSECTION_PARSER_HPP

#include <cstdint>
#include <stdio.h>

#include "decompression.hpp"

class CompressedsectionParser
    : public Decompression
{
private:
    int32_t last_number;
public:
    CompressedsectionParser()
        : Decompression()
        , last_number(0) { }

    virtual int try_parse
    (
        const char* buffer,
        int offset,
        int count
    );

    virtual int parse
    (
        const char* buffer,
        int offset,
        int count
    );
};

#endif /* end of include guard: COMPRESSEDSECTION_PARSER_HPP */

