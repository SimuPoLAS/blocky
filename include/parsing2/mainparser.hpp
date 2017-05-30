#ifndef MAINPARSER_HPP
#define MAINPARSER_HPP

#include <parsing/parsers/parser.hpp>

class MainParser
{
private:
    Data data;
    std::vector<Parser> parsers;

public:
    Parser();

    parse
    (
        const TokenType* buffer,
        int offset,
        int count
    );
};

#endif /* end of include guard: MAINPARSER_HPP */
