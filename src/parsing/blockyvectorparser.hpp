#ifndef BLOCKY_VECTOR_PARSER_HPP
#define BLOCKY_VECTOR_PARSER_HPP

#include "parser.hpp"

class BlockyVectorParser
    : public Parser
{
public:
	BlockyVectorParser(Hooker& hooker)
        : Parser(hooker) { }

    virtual int try_parse
    (
        const char* buffer,
        int offset,
        int count
    );

    virtual int parse_constant
    (
        const char* buffer,
        int offset,
        int count
    );
};

#endif /* end of include guard: BLOCKY_PARSER_HPP */
