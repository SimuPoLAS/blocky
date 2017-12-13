#ifndef BLOCKY_TENSOR_PARSER_HPP
#define BLOCKY_TENSOR_PARSER_HPP

#include "parser.hpp"

class BlockyTensorParser
    : public Parser
{
public:
    BlockyTensorParser(Hooker& hooker)
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

#endif /* end of include guard: BLOCKY_TENSOR_PARSER_HPP */
