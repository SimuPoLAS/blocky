#ifndef LIST_PARSER_HPP
#define LIST_PARSER_HPP

#include <memory>

#include <parsing/parsers/parser.hpp>
#include <parsing/parsers/blockyparser.hpp>

class ListParser
    : public Parser
{
private:
    /*
        This parser is used for parsing the variable part
        The variable part for the listparser is the contant part for this parser
    */
    std::unique_ptr<BlockyParser> blockyParser;

public:
    /*
        The data in buffer MUST look like this in order to be parsed:
        KEYWORD {List}
        CHEVRONS_OPEN
        KEYWORD {scalar}|{vector}|{tensor}
        CHEVRONS_CLOSE
        (NUMBER)
        PARANTHESE_OPEN
    */
    virtual bool try_parse
    (
        const Token* buffer,
        int offset,
        int count
    ) override;

    /*
        Two major sets are made:
        * the list_element_parser, who will parse the list elements
        * (optional) the given size of the list (MAY not be the actual size)
    */
    virtual int parse_constant
    (
        const Token* buffer,
        int& offset,
        int& count
    ) override;

    /*
        Parses the variable part of the record by using the list_element_parser
    */
    virtual int parse_variable
    (
        const Token* buffer,
        int& offset,
        int& count
    ) override;
};

#endif /* end of include guard: LIST_PARSER_HPP */
