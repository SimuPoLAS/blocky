#ifndef BLOCKY_TENSOR_PARSER_HPP
#define BLOCKY_TENSOR_PARSER_HPP

#include "parser.hpp"
#include "blockyscalarparser.hpp"

class BlockyTensorParser
    : public Parser
{
public:
    BlockyTensorParser(Hooker& hooker)
        : Parser(hooker)
	{
		scalarParser = std::make_unique<BlockyScalarParser>(hooker);
	}
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
	std::unique_ptr<BlockyScalarParser> scalarParser;
};

#endif /* end of include guard: BLOCKY_TENSOR_PARSER_HPP */
