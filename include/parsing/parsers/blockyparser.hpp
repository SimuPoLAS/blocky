#ifndef BLOCKY_PARSER_HPP
#define BLOCKY_PARSER_HPP

#include <parsing/parsers/parser.hpp>

class BlockyParser
    : public Parser
{
private:
    int c;

public:
    BlockyParser(Data& data, int c);

    int count() { return c; }

    /*
        The data in buffer MUST look like this in order to be parsed:
        NUMBER * count

        This means that number MUST be as many times available as counts content

        *count: the variable that determines the length of one unit in numbers
    */
    virtual bool try_parse
    (
        const Token* buffer,
        int offset,
        int count
    ) override;

    /*
        One major set is made:
        * the numbers (quantity determined my count) will be added to data
    */
    virtual int parse_constant
    (
        const Token* buffer,
        int& offset,
        int& count
    ) override;

};

#endif /* end of include guard: BLOCKY_PARSER_HPP */
