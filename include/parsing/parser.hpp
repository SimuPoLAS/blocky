#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include <parsing/token.hpp>
#include <parsing/lexer.hpp>
#include <parsing/hooker.hpp>

using namespace std;

class Parser {
private:
    static const int bufferSize = 4096;

    Token buffer[bufferSize];
    bool eos;
    int32_t length;
    int32_t position;
    int32_t size;
    Lexer lexer;

public:
    Parser();
    ~Parser();
};

#endif /* end of include guard: PARSER_HPP */
