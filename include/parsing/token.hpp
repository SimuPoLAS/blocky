#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <parsing/tokentype.hpp>

using namespace std;

struct Token
{
    TokenType type;
    string payload;
    uint32_t position;
    uint32_t column;
    uint32_t length;
    uint32_t line;

    Token()
        : type(TokenType::NONE)
        , payload("")
        , position(0) {}

    Token(TokenType type)
        : type(type)
        , payload("")
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

    string to_s()
    {
        string s;
        s += "Type: ";
        switch (type) {
            case TokenType::BRACES_CLOSE:
                s += "Braces Close";
                break;
            case TokenType::BRACES_OPEN:
                s += "Braces Open";
                break;
            case TokenType::BRACKETS_CLOSE:
                s += "Brackets Close";
                break;
            case TokenType::BRACKETS_OPEN:
                s += "Brackets Open";
                break;
            case TokenType::CHEVRONS_CLOSE:
                s += "Chevrons Close";
                break;
            case TokenType::CHEVRONS_OPEN:
                s += "Chevrons Open";
                break;
            case TokenType::END_OF_STREAM:
                s += "End of Stream";
                break;
            case TokenType::HASHTAG:
                s += "Hashtag";
                break;
            case TokenType::KEYWORD:
                s += "Keyword";
                break;
            case TokenType::NONE:
                s += "None";
                break;
            case TokenType::NUMBER:
                s += "Number";
                break;
            case TokenType::PARENTHESE_CLOSE:
                s += "Parenthese Close";
                break;
            case TokenType::PARENTHESE_OPEN:
                s += "Parenthese Open";
                break;
            case TokenType::SEMICOLON:
                s += "Semicolon";
                break;
            case TokenType::STRING:
                s += "String";
                break;
            default:
                s += "Idk";
        }
        s += '\n';
        s += "Payload: " + payload;
        s += "\n";
        s += "Position: " + to_string(position);
        return s;
    }
};

#endif /* end of include guard: TOKEN_HPP */
