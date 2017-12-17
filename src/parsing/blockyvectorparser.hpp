#ifndef BLOCKY_VECTOR_PARSER_HPP
#define BLOCKY_VECTOR_PARSER_HPP

#include "parser.hpp"
#include "blockyscalarparser.hpp"

class BlockyVectorParser
    : public Parser
{
public:
	BlockyVectorParser(Hooker& hooker)
        : Parser(hooker) { }

    virtual int try_parse
    (
        const char* buffer,
        int offset,
        int count
    );

    virtual int parse_constant
    (
        const char* buffer,
        int offset,
        int count
    );
private:
	std::unique_ptr<BlockyScalarParser> scalarParser = make_unique<BlockyScalarParser>();
};

#endif /* end of include guard: BLOCKY_PARSER_HPP */
