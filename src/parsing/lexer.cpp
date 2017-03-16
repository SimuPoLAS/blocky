#include <cstring>
#include <functional>
#include <iostream>

#include <parsing/lexer.hpp>

Lexer::Lexer(char (&buffer)[bufferSize], bool recordPosition)
	: buffer(buffer)
    , length(0)
    , position(0)
    , size(bufferSize)
	, recordPosition(recordPosition)
	, textBuffer(NULL)
    , textPosition(0)
    , textLength(0)
    , textSize(64)
	, currentPosition(0)
	, tokenLine(0)
	, tokenColumn(0)
	, tokenPosition(0)
	, symbolOnly(false)
{
    eos = false;
    textBuffer = new char[textSize];
}

void Lexer::double_buffer()
{
    textSize *= 2;
    char* newbuf = new char[textSize];
    memcpy((void*)newbuf, (void*)textBuffer, textSize/2);
    textBuffer = newbuf;
}

void Lexer::record(char c)
{
    if (c == '\n')
    {
        currentLine++;
        currentColumn = 1;
        return;
    }
    if (c == '\t')
    {
        currentColumn += tabSize;
        return;
    }
    currentColumn++;
}

void Lexer::record(uint32_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (buffer[position + i] == '\n')
        {
            currentLine++;
            currentColumn = 1;
            continue;
        }
        if (buffer[position + i] == '\t')
        {
            currentColumn += tabSize;
            continue;
        }
        currentColumn++;
    }
}

void Lexer::append(char value)
{
    if (textPosition == textSize)
        double_buffer();
    textBuffer[textPosition++] = value;
    textLength++;
}

void Lexer::clear()
{
    textLength = textPosition = 0;
}

shared_ptr<Token> Lexer::create_token(TokenType type)
{
    return create_token(type, "");
}

shared_ptr<Token> Lexer::create_token(TokenType type, string data)
{
    return make_shared<Token>
    (
        type,
        data,
        tokenPosition,
        tokenLine,
        tokenColumn,
        currentPosition - tokenPosition
    );
}

void Lexer::start_token()
{
    if (!recordPosition) return;
    tokenLine = currentLine;
    tokenColumn = currentColumn;
    tokenPosition = currentPosition;
}

char Lexer::eat()
{
    auto character = buffer[position];
    length--;
    position++;
    currentPosition++;
    if (recordPosition) record(character);
    return character;
}

bool Lexer::eat_until(char const* values, int size)
{
    clear();
    if (size == 0) return true;
    int z = 0, l = size;
    while (!(eos && length == 0))
    {
        if (length == 0) throw 404;
        for (size_t i = 0; i < length; i++)
        {
            auto v = buffer[position + i];
            if (v == values[z])
            {
                z++;
                if (z == l)
                {
                    auto r = l - 1;
                    if (r < 0) r = 0;
                    textLength -= r;
                    skip(i + 1);
                    return true;
                }
            }
            else z = 0;
            append(v);
        }
        reset();
    }
    return false;
}

void Lexer::fill()
{
    // not supported
    std::cout << "fill called" << '\n';
}

bool Lexer::needs(int32_t amount)
{
    if (amount <= length) return true;
    if (eos) return false;
    throw 404;
    return amount <= length;
}

void Lexer::reset()
{
    if (recordPosition) record((uint32_t)length);
    currentPosition += (uint32_t)length;
    length = 0;
}

void Lexer::skip()
{
    if (recordPosition) record(buffer[position]);
    currentPosition++;
    position++;
    length--;
}

void Lexer::skip(int32_t amount)
{
    if (recordPosition) record((uint32_t)amount);
    currentPosition += (uint32_t)amount;
    position += amount;
    length -= amount;
}

void Lexer::relocate()
{
    // not supported
}

void Lexer::skip_until(char value)
{
    while (!(eos && length == 0))
    {
        if (length == 0) throw 404;
        for (size_t i = 0; i < length; i++)
        {
            if (buffer[position + i] != value) continue;
            skip(i + 1);
            return;
        }
        reset();
    }
}

bool Lexer::skip_until(char const* values, int size)
{
    if (size == 0) return true;
    int z = 0, d = 0, l = size;
    while (!(eos && length == 0))
    {
        if (length == 0) throw 404;
        for (size_t i = 0; i < length; i++)
        {
            if (buffer[position + i] == values[z])
            {
                z++;
                if (z != l) continue;
                skip(i + 1);
                return true;
            }
            if (z != 0) i = d;
            d = i + 1;
            z = 0;
        }
        reset();
    }
    return false;
}

TokenType Lexer::is_single_char_token(char c)
{
    switch (c)
    {
        case '{':
            return TokenType::BRACES_OPEN;
        case '}':
            return TokenType::BRACES_CLOSE;
        case '[':
            return TokenType::BRACKETS_OPEN;
        case ']':
            return TokenType::BRACKETS_CLOSE;
        case '(':
            return TokenType::PARENTHESE_OPEN;
        case ')':
            return TokenType::PARENTHESE_CLOSE;
        case '<':
            return TokenType::CHEVRONS_OPEN;
        case '>':
            return TokenType::CHEVRONS_CLOSE;
        case ';':
            return TokenType::SEMICOLON;
        case '#':
            return TokenType::HASHTAG;
        default:
            return TokenType::NONE;
    }
}

void Lexer::eat_keyword()
{
    // while there are still chars available
    while (!(eos && length == 0))
    {
        // if there are no more chars in the buffer read new
        if (length == 0)
            throw 404;
        // go through all items in the buffer, check if the follow the rule
        for (uint16_t i = 0; i < length; i++)
        {
            auto c = buffer[position + i];
            if (isspace(c) || is_single_char_token(c) != TokenType::NONE)
            {
                skip(i);
                return;
            }
            append(c);
        }
        reset(); // all the chars in the buffer are valid -> skip them
    }
}

void Lexer::eat_number(bool ignoreEmpty)
{
    // while there are still characters availble
    auto amount = 0;
    while (!(eos && length == 0))
    {
        // exit when buffer is emtpy
        if (length == 0)
            throw 404;
        // loop though all available characters in the buffer
        char c;
        for (uint16_t i = 0; i < length; i++, amount++)
            if ((c = buffer[position + i]) >= '0' && c <= '9')
                append(c);
            else
            {
                skip(i); // eat all read chars
                if (!ignoreEmpty && amount == 0)
                   throw 0;
                return;
            }
        reset(); // eat all chars in the buffer
    }
    if (amount == 0)
        throw 0;
}

int32_t Lexer::eat_string()
{
    // while there are still characters availble
    auto amount = 0;
    auto escape = false;
    while (!(eos && length == 0))
    {
        // exit when buffer is empty
        if (length == 0)
            throw 404;
        // loop though all available characters in the buffer
        for (uint16_t i = 0; i < length; i++, amount++)
        {
            auto c = buffer[position + i];
            if (escape)
            {
                escape = false;
                switch (c)
                {
                    case '"':
                        append('"');
                        break;
                    case '\\':
                        append('\\');
                        break;
                    case '/':
                        append('/');
                        break;
                    case 'b':
                        append('\b');
                        break;
                    case 'f':
                        append('\f');
                        break;
                    case 'n':
                        append('\n');
                        break;
                    case 'r':
                        append('\r');
                        break;
                    case 't':
                        append('\t');
                        break;
                    default:
                        throw 0;
                }
            }
            else if (c == '\\') escape = true;
            else if (c == '\n')
                throw 0;
            else if (c != '"') append(c); // if the current char matches the Regex add it to the StringBuilder
            else
            {
                skip(i); // eat all read chars
                return amount; // return the amount of read chars
            }
        }
        reset(); // eat all chars in the buffer (we have read them)
    }
    return -1; // return the amount of read chars
}

shared_ptr<Token> Lexer::next_token()
{
    try
    {
        //Position++;
        while (needs(1))
        {
            // Save the next character store symbol only and set it to false
            auto c = buffer[position];
            auto onlySymbols = symbolOnly;
            symbolOnly = false;

            // If the current character is a whitespace skip it
            if (isspace(c))
            {
                skip();
                continue;
            }

            // Check if the current char is the start of a comment
            if (c == '/' && needs(2))
            {
                c = buffer[position + 1];
                if (c == '/') // Handling single line comment
                {
                    skip_until('\n');
                    continue;
                }
                if (c == '*') // Handling multi line comment
                {
                    if (!skip_until("*/", 2))
                        throw 0;
                    continue;
                }
            }

            // Save the current position as starting position for the token
            start_token();

            // Lexing string containers #{.*?#}
            if
            (
                !onlySymbols &&
                c == '#' &&
                needs(2) &&
                buffer[position + 1] == '{')
            {
                skip(2);
                if (!eat_until("#}", 2))
                    throw 0;
                symbolOnly = true;
                return create_token
                (
                    TokenType::STRING,
                    string(textBuffer, textLength)
                );
            }

            // Check if the current char is a token
            auto singleTokenType = is_single_char_token(c);
            if (singleTokenType != TokenType::NONE)
            {
                skip(1);
                return create_token(singleTokenType);
            }

            // If the last character was not a symbol, whitespace or comment and this one isn't too -> BAIL!!!
            if (onlySymbols)
                throw 0;
            symbolOnly = true;

            // Lexing Numbers reg. -?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)? - for visualization -> json.org
            if (isdigit(c) || c == '-')
            {
                clear();
                if (c == '-')
                    append(eat());
                if (!needs(1))
                    throw 0;
                c = buffer[position];
                if (c == '0') append(eat());
                else if (c >= '1' && c <= '9')
                {
                    append(eat());
                    eat_number(true);
                }
                else
                    throw 0;
                if (!needs(1))
                    return create_token
                    (
                        TokenType::NUMBER,
                        string(textBuffer, textLength)
                    );
                c = buffer[position];
                if (c == '.')
                {
                    append(eat());
                    eat_number(false);
                    if (!needs(1))
                        return create_token
                        (
                            TokenType::NUMBER,
                            string(textBuffer, textLength)
                        );
                    c = buffer[position];
                }
                if (c == 'e' || c == 'E')
                {
                    append(eat());
                    if (!needs(1))
                        throw 0;
                    c = buffer[position];
                    if (c == '+' || c == '-')
                        append(eat());
                    eat_number(false);
                }

                return create_token
                (
                    TokenType::NUMBER,
                    string(textBuffer, textLength)
                );
            }

            // Lexing strings reg. ".*?"
            if (c == '"')
            {
                clear();
                skip(1);
                if (eat_string() == -1)
                    throw 0;
                skip(1);
                return create_token
                (
                    TokenType::STRING,
                    string(textBuffer, textLength)
                );
            }

            // Lexing Keywords reg. [a-zA-Z_$]([a-zA-Z0-9_]|'(')*
            clear();
            append(eat());
            eat_keyword();
            return create_token
            (
                TokenType::KEYWORD,
                string(textBuffer, textLength)
            );
        }
        return create_token(TokenType::END_OF_STREAM);
    }
    catch(int e)
    {
        //if (e == 404)
            //std::cout << "buffer must be reload" << '\n';

        return nullptr;
    }
}

int Lexer::read(shared_ptr<Token> buf[], int w, int& processed)
{
    position = 0;
    length = w;

    if (buf == nullptr)
        return -1;

    size_t i = 0;
    for (; i < bufferSize && length != 0; i++)
    {
        auto tkn = next_token();
        if (tkn == nullptr)
            break;
        processed = position;
        if (tkn->Type == TokenType::END_OF_STREAM)
            break;
        buf[i] = tkn;
    }
    return i;
}
