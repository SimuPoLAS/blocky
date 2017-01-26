#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>

#include <token.hpp>

using namespace std;

class Parser {
private:
    vector<Token> buffer;
    bool eos;
    int32_t length;
    int32_t position;
    int32_t size;

public:
    Parser();
    ~Parser();
};

#endif /* end of include guard: PARSER_HPP */
