#ifndef DECOMPRESSION_HPP
#define DECOMPRESSION_HPP

#define TRY_PARSE_OK 0
#define TRY_PARSE_BUFFER_SHORT -1
#define TRY_PARSE_INVALID -2

#include <vector>

class Decompression
{
public:
    Decompression() { }

    virtual int try_parse
    (
        const char* buffer,
        int offset,
        int count
    ) = 0;

    virtual int parse
    (
        const char* buffer,
        int offset,
        int count
    ) { return -404; } // TODO: throw meaningful errors
};

#endif /* end of include guard: DECOMPRESSION_HPP */