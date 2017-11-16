#ifndef CHAR_PARSER_HPP
#define CHAR_PARSER_HPP

#include "parser.hpp"

class CharParser
    : public Parser
{
public:
    CharParser(Hooker& hooker)
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

#endif /* end of include guard: CHAR_PARSER_HPP */
