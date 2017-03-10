#ifndef LIST_PARSER_HPP
#define LIST_PARSER_HPP

#include <parser>

class ListParser
    : public Parser
{
private:
    std::unique_ptr<Parser> list_element_parser;

public:
    /* This method checks, if the data provided is valid
     * Whether its valid or not, this parser will parse them or not
     *
     * The data in buffer should look like this in order to
     * be parsed by this parser:
     * KEYWORD {List}
     * CHEVRONS_OPEN
     * KEYWORD {scalar}|{vector}|{tensor}
     * CHEVRONS_CLOSE
     * (NUMBER)
     * PARANTHESE_OPEN
     */
    virtual bool try_parse
    (
        const TokenType* buffer,
        int offset,
        int count
    ) override
    {
        // the provided buffer MUST at least have five tokens in
        if (count < 5)
            return false;

        // the first token MUST be a keyword
        if (buffer[offset].Type != TokenType::KEYWORD)
            return false;

        // the first token MUST have the "List" payload
        if (buffer[offset].Payload != "List")
            return false;

        // the second token MUST be an open chevron
        if (buffer[offset + 1].Type != TokenType::CHEVRONS_OPEN)
            return false;

        // the third token MUST be a keyword
        if (buffer[offset + 2].Type != TokenType::KEYWORD)
            return false;

        // the fourth token MUST be a closed chevron
        if (buffer[offset + 3].Type != TokenType::CHEVRONS_CLOSE)
            return false;

        auto ltype = buffer[offset + 2].Payload

        // the second token MUST have the
        // "scalar" or "vector" or "tensor" payload
        if (ltype != "scalar" && ltype != "vector" && ltype != "tensor")
            return false;

        // the fifth token MAY be a number
        if (buffer[offset + 4].Type == TokenType::NUMBER)
        {
            // if it is, then other conditions apply

            // the provided buffer MUST at least have 6 tokens in
            if (count < 6)
                return false;

            // the sixth token MUST be an open paranthese
            if (buffer[offset + 5].Type != TokenType::PARANTHESE_OPEN)
                return false;
        }
        // if the fifth token is not a number then
        // the fifth token MUST be an open paranthese
        else if (buffer[offset + 4].Type != TokenType::PARANTHESE_OPEN)
            return false;

        // if all checks passed, the provided buffer can be parsed
        return true;
    }

    /* This method makes the data ready to be filled by the parser method
     * Since the parser method may be called multiple times
     * due to the fact, that the buffer could end while parsing
     * and that there are some variables that has to be set only once
     *
     * This method may be not used at all, because this parser
     * may not support continuous parsing
     *
     * *continuous = parsing though multiple buffers
     */
    virtual void prepare
    (
        const TokenType* buffer,
        int offset,
        int count
    ) override
    {
        auto ltype = buffer[offset + 2].Payload
        int multiplier = 1;

        if (ltype == "scalar")
            list_element_parser{ new ScalarListElementParser(data) };
        else if (ltype == "vector")
            list_element_parser{ new VectorListElementParser(data) };
        else if (ltype == "tensor")
            list_element_parser{ new TensorListElementParser(data) };

        if (buffer[offset + 4].Type == TokenType::NUMBER)
        {
            auto amount = (int)stod(buffer[offset + 4].Payload);
            data.resize_numbers(amount);
        }
    }

    virtual int parse
    (
        const TokenType* buffer,
        int offset,
        int count
    ) override
    {

    }
}

#endif /* end of include guard: LIST_PARSER_HPP */
