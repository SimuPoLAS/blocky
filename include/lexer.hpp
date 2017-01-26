#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <memory>

#include <token.hpp>

using namespace std;

class Lexer {
private:
    static const int bufferSize = 4096;

    /* base members */
    char (&buffer)[bufferSize];
    bool eos;
    int32_t length;
    int32_t position;
    int32_t size;

    /* lexer base members */
    bool recordPosition;
    uint32_t tabSize = 4;
    char* textBuffer;
    int32_t textPosition;
    int32_t textLength;
    int32_t textSize = 4;

    uint32_t currentLine = 1;
    uint32_t currentColumn = 1;
    uint32_t currentPosition;

    uint32_t tokenLine;
    uint32_t tokenColumn;
    uint32_t tokenPosition;

    /* lexer specific members */
    bool symbolOnly;

    /* base lexer methods */
    void double_buffer();
    void record(char c);
    void record(uint32_t count);
    void append(char value);
    void clear();
    Token create_token(TokenType type);
    Token create_token(TokenType type, string data);
    void start_token();
    char eat();
    bool eat_until(char const*values, int size);
    void fill();
    bool needs(int32_t amount);
    void reset();
    void skip();
    void skip(int32_t amount);
    void relocate();
    void skip_until(char value);
    bool skip_until(char const*values, int size);

    /* lexer methods */
    static TokenType is_single_char_token(char c);
    void eat_keyword();
    void eat_number(bool ignoreEmpty);
    int32_t eat_string();

public:
    Lexer(char (&buffer)[bufferSize], bool recordPosition);

    Lexer(char (&buffer)[bufferSize])
        : Lexer(buffer, true) {}

    ~Lexer();

    Token next_token();
    // int32_t read(Token[] buf, int offset, int count);
};

#endif /* end of include guard: LEXER_HPP */
