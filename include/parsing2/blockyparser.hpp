#ifndef BLOCKY_PARSER_HPP
#define BLOCKY_PARSER_HPP

#include <parsing2/parser.hpp>

class BlockyParser
    : public Parser
{
public:
    BlockyParser(Hooker& hooker)
        : Parser(hooker) { }

    /*
        The data in buffer MUST look like this in order to be parsed:
        \d+\.?\d*\s

        This means that number MUST be as many times available as counts content

        *count: the variable that determines the length of one unit in numbers
    */
    virtual bool try_parse
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

#endif /* end of include guard: BLOCKY_PARSER_HPP */
