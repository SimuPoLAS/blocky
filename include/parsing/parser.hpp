#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>

#include <parsing/token.hpp>
#include <parsing/lexer.hpp>
#include <parsing/hooker.hpp>
#include <parsing/positionprovider.hpp>

using namespace std;

enum class ParserResult
{
    SUCCESS = 0,
    END_OF_BUFFER = 1,
    UNEXPECTED_TOKEN = -1,
    UNEXPECTED_EOF = -2
};

class Parser
    : public PositionProvider
{
private:
    // TODO: make bufferSize globaly available
    static const int bufferSize = 4096;

    shared_ptr<Token> (&buffer)[bufferSize];
    bool eos;
    int32_t length;
    int32_t position;
    int32_t size;
    int32_t filePos;
    unique_ptr<Hooker> hooker;

    // base parser methods
    // TODO: decide whether to use int32_t or size_t for amount
    ParserResult ensure(int32_t amount);
    void fill();
    ParserResult needs(int32_t amount);
    void skip();
    void skip(int32_t amount);
    // void relocate();

    // parser methods
    ParserResult expect(TokenType type);
    ParserResult expect(TokenType types[], size_t count);

    ParserResult parse_entry_or_object(Token const& me);
    ParserResult parse_object(Token const& me);
    ParserResult parse_code_stream_object(Token const& me);
    ParserResult parse_entry(Token const& me);
    ParserResult parse_directive();
    ParserResult parse_value(Token const& me);
    ParserResult parse_list(ListType type, int32_t amount);
    ParserResult parse_list_continue(ListType type);
    ParserResult parse_anonymous_list(int32_t number = -1);
    ParserResult parse_scalar();
    ParserResult parse_vector();
    ParserResult parse_tensor();

public:
    bool SimpleAnonyomousLists;

    Parser
    (
        FILE* file,
        shared_ptr<Token> (&buffer)[bufferSize]
    );

    uint32_t get_position() override;
    void parse(int w);
};

#endif /* end of include guard: PARSER_HPP */
