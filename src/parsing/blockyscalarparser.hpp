#ifndef BLOCKY_SCALAR_PARSER_HPP
#define BLOCKY_SCALAR_PARSER_HPP

#include "parser.hpp"

class BlockyScalarParser
    : public Parser
{
public:
    BlockyScalarParser(Hooker& hooker)
        : Parser(hooker) { }

    /*
        The data in buffer MUST look like this in order to be parsed:
        \d+\.?\d*\s

        This means that number MUST be as many times available as counts content

        *count: the variable that determines the length of one unit in numbers
    */
    virtual int try_parse
    (
        const char* buffer,
        int offset,
        int count
    );

    /*
        One major set is made:
        * the number will be added to hooker
    */
    virtual int parse_constant
    (
        const char* buffer,
        int offset,
        int count
    );

};

#endif /* end of include guard: BLOCKY_SCALAR_PARSER_HPP */
