#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>

#include <parsing/token.hpp>
#include <parsing/lexer.hpp>
#include <parsing/hooker.hpp>
#include <parsing/positionprovider.hpp>

using namespace std;

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
    void ensure(int32_t amount);
    void fill();
    bool needs(int32_t amount);
    void skip();
    void skip(int32_t amount);
    // void relocate();

    // parser methods
    bool expect(TokenType type);
    bool expect(TokenType types[], size_t count);
    bool expect_safe(TokenType types[], size_t count);

    void parse_entry_or_object(Token const& me);
    void parse_object(Token const& me);
    void parse_code_stream_object(Token const& me);
    void parse_entry(Token const& me);
    void parse_directive();
    void parse_value(Token const& me);
    void parse_list(ListType type, int32_t amount);
    void parse_anonymous_list(int32_t number = -1);
    void parse_scalar();
    void parse_vector();
    void parse_tensor();

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
