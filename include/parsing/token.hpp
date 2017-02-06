#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <parsing/tokentype.hpp>

using namespace std;

struct Token
{
    TokenType Type;
    string Payload;
    uint32_t Position;
    uint32_t Column;
    uint32_t Length;
    uint32_t Line;

    Token()
        : Type(TokenType::NONE)
        , Payload("")
        , Position(0) { }

    Token(TokenType type)
        : Type(type)
        , Payload("")
        , Position(0) { }

    Token(TokenType type, string payload, uint32_t position)
        : Type(type)
        , Payload(payload)
        , Position(position)
        , Column(0)
        , Length(0)
        , Line(0) { }

    Token
    (
        TokenType type,
        string payload,
        uint32_t position,
        uint32_t column,
        uint32_t length,
        uint32_t line
    )
        : Type(type)
        , Payload(payload)
        , Position(position)
        , Column(column)
        , Length(length)
        , Line(line) { }

    string to_s() const
    {
        string s;
        s += "Type: ";
        switch (Type) {
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
        s += "Payload: " + Payload;
        s += "\n";
        s += "Position: " + to_string(Position);
        return s;
    }
};

#endif /* end of include guard: TOKEN_HPP */
