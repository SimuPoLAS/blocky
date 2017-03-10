#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <vector>

#include <parsing/tokentype.hpp>
#include <parsing/parsers/data.hpp>

class Parser
{
protected:
    Data& data;

public:
    Parser(Data& data)
        : data(data) { }

    virtual bool try_parse
    (
        const TokenType* buffer,
        int offset,
        int count
    ) = 0;

    virtual void prepare
    (
        const TokenType* buffer,
        int offset,
        int count
    ) = 0;

    virtual int parse
    (
        const TokenType* buffer,
        int offset,
        int count
    ) = 0;
};

#endif /* end of include guard: PARSER_HPP_ */
