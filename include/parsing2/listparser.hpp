#ifndef LIST_PARSER_HPP
#define LIST_PARSER_HPP

#include <memory>

#include <parsing/listtype.hpp>
#include <parsing2/parser.hpp>
#include <parsing2/blockyparser.hpp>

class ListParser
    : public Parser
{
private:
    /*
        This parser is used for parsing the variable part
        The variable part for the listparser is the contant part for this parser
    */
    std::unique_ptr<BlockyParser> blockyParser;

    /*

    */
    bool end = true;

public:
    ListParser(Hooker& hooker)
        : Parser(hooker) { }

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
        const char* buffer,
        int offset,
        int count
    );

    /*
        Two major sets are made:
        * the list_element_parser, who will parse the list elements
        * (optional) the given size of the list (MAY not be the actual size)
    */
    virtual int parse_constant
    (
        const char* buffer,
        int offset,
        int count
    );

    /*
        Parses the variable part of the record by using the list_element_parser
    */
    virtual int parse_variable
    (
        const char* buffer,
        int offset,
        int count
    );
};

#endif /* end of include guard: LIST_PARSER_HPP */
