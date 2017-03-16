// TODO: remove underline
#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <vector>

#include <parsing/token.hpp>
#include <parsing/parsers/data.hpp>

/*
    This class is responsible for parsing records

    Records consist of two parts:
    1. constant part
    2. variable part

    constant part:
    * MUST be available
    * its size MUST be fixed

    variable part:
    * MAY be available
    * its size MUST be dynamic
        * its end MUST be defined by an escape sequence
    * MAY span over the size of a buffer
*/
class Parser
{
protected:
    /*

    */
    Data& data;

public:
    Parser(Data& data)
        : data(data) { }

    /*
        This method decides whether the provided buffer can be parsed or not
        It communicates this information via the return bool

        It MUST be called first
    */
    virtual bool try_parse
    (
        const Token* buffer,
        int offset,
        int count
    ) = 0;

    /*
        This method parses the constant part of the provided record

        If the record spans over the size of a buffer:
        * it MUST called only once at the beginning of the record

        It MUST be called before parse_variable()
    */
    virtual int parse_constant
    (
        const Token* buffer,
        int& offset,
        int& count
    ) = 0;

    /*
        This method parses the varable part of the provided record

        If the record spans over the size of a buffer:
        * it MUST called every time the buffer is refilled

        It MUST be called after parse_constant()
    */
    virtual int parse_variable
    (
        const Token* buffer,
        int& offset,
        int& count
    ) { return 0; }
};

#endif /* end of include guard: PARSER_HPP_ */
