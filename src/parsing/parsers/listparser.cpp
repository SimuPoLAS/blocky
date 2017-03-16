#include <parsing/parsers/listparser.hpp>

bool ListParser::try_parse
(
    const Token* buffer,
    int offset,
    int count
)
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

    auto ltype = buffer[offset + 2].Payload;

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
        if (buffer[offset + 5].Type != TokenType::PARENTHESE_OPEN)
            return false;
    }
    // if the fifth token is not a number then
    // the fifth token MUST be an open paranthese
    else if (buffer[offset + 4].Type != TokenType::PARENTHESE_OPEN)
        return false;

    // if all checks passed, the provided buffer can be parsed
    return true;
}

int ListParser::parse_constant
(
    const Token* buffer,
    int& offset,
    int& count
)
{
    if (count <= 0)
        // TODO: return unexpected end of buffer
        return -1;

    auto ltype = buffer[offset + 2].Payload;
    int blockyCount;

    if (ltype == "scalar")
        blockyCount = 1;
    else if (ltype == "vector")
        blockyCount = 3;
    else if (ltype == "tensor")
        blockyCount = 9;
    else
        // TODO: return meaningful codes
        return -1;

    blockyParser = std::make_unique<BlockyParser>(data, blockyCount);

    if (buffer[offset + 4].Type == TokenType::NUMBER)
    {
        auto amount = (int)stod(buffer[offset + 4].Payload);
        data.resize_numbers(amount * blockyCount);
        offset++;
        count--;
    }

    offset += 5;
    count -= 5;
    return 0;
}

int ListParser::parse_variable
(
    const Token* buffer,
    int& offset,
    int& count
)
{
    if (count <= 0)
        // TODO: return expected end of buffer result
        return 1;

    int result = 0;
    while (result == 0)
    {
        // trying to parse the variable record
        if (blockyParser->try_parse(buffer, offset, count))
        {
            // if it is parsable, then parse
            result = blockyParser->parse_constant
            (
                buffer,
                offset,
                count
            );
        }
        else
        {
            // if not, checking whether it is an end of buffer or not
            if (count <= blockyParser->count())
                // TODO: return expected end of buffer
                return 1;

            // when there is no end of buffer, then the problem is
            // an unexpected token
            // TODO: return unexpected token
            return -1;
        }

        // after parsing is done, we check for the escape sequence
        if (buffer[offset].Type == TokenType::PARENTHESE_OPEN)
            // if it is, get out of the loop
            break;
    }

    return result;
}
