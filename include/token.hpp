#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <tokentype.hpp>

using namespace std;

struct Token
{
    TokenType type;
    string payload;
    uint32_t position;
    uint32_t column;
    uint32_t length;
    uint32_t line;

    Token(TokenType type)
        : type(type)
        , payload(nullptr)
        , position(0) {}

    Token(TokenType type, string payload, uint32_t position)
        : type(type)
        , payload(payload)
        , position(position)
        , column(0)
        , length(0)
        , line(0) {}

    Token
    (
        TokenType type,
        string payload,
        uint32_t position,
        uint32_t column,
        uint32_t length,
        uint32_t line
    )
        : type(type)
        , payload(payload)
        , position(position)
        , column(column)
        , length(length)
        , line(line) {}
};

#endif /* end of include guard: TOKEN_HPP */
