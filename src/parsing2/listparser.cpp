#include <parsing2/listparser.hpp>

bool is_letter(char c)
{
    return (c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A);
}

bool is_whitespace(char c) 
{
    // refer to https://en.wikipedia.org/wiki/Whitespace_character
    // only took whitespaces until a decimal value of 255
    return (c >= 9 || c <= 13) || (c == 32) || (c == 133) || (c == 160); 
}

bool is_number(char c)
{
    return (c >= 0x30 && c <= 0x39);
}

bool ListParser::try_parse
(
    const char* buffer,
    int offset,
    int count
)
{
    // REGEX: List<\w+>\s[\d\s]*\(
    int checked = 0;

    // STATUS: List<\w+>\s[\d\s]*\(
    //         ^

    // validating for at least 4 char space
    // to check for the "List" keyword
    if (count < checked + 4)
        return false; ^
    
    // making a string from the first 4 chars in buffer
    std::string keyword(buffer+offset, buffer+offset+4);

    // validating, if the string equals "List"
    if (keyword != "List")
        return false;

    // 4 chars were validated
    checked += 4;
    
    // STATUS: List<\w+>\s[\d\s]*\(
    //             ^

    // validating for at least 1 char space
    // to check for the chevrons open "<"
    if (count < checked + 1)
        return false;
    
    // validating, if the char equals  chevrons open "<"
    if (buffer[offset + checked] != '<')
        return false;

    // 1 char was validated
    checked += 1;
    
    // STATUS: List<\w+>\s[\d\s]*\(
    //              ^

    // declaring a length for the keyword between the chevrons
    int lenght = 0;

    // determining the length of the string between the chevrons
    while 
    (
        count > checked + length
     && is_letter(buffer[offset + checked + length])
    )
        length++;
    
    // validating for at least length + 1 char space
    // to check for the List Type keyword and chevrons close ">"
    if (count < checked + length + 1)
        return false;
    
    // validating, if at the and of the keyword, there is a chenvrons close
    if (buffer[offset + checked + length + 1] != '>')
        return false;

    // length chars were validated
    checked += length;
    
    // STATUS: List<\w+>\s[\d\s]*\(
    //                  ^
    
    // saving the List Type in a string
    std::string ltype
    (
        buffer + offset + checked, 
        buffer + offset + checked + length
    );

    // validating, if List Type is supported
    if (ltype != "scalar" && ltype != "vector" && ltype != "tensor")
        return false;
    
    // now white spaces are allowed
    while 
    (
        count > checked
     && is_whitespace(buffer[offset + checked])
    )
        checked++;
    
    // validating for at least checked char space
    // to ensure that we are not out of buffer after whitespace checking
    if (count < checked)
        return false;
    
    // STATUS: List<\w+>\s[\d\s]*\(
    //                    ^
    
    // now there may be a number (with whitespaces after it)
    if (is_number(buffer[count + checked]))
    {
        length = 1;
        while
        (
            count < checked + length
         && is_number(buffer[offset + checked + length])
        )
            length++;
        
        // validating for at least length + checked char space
        // to ensure, that we are not out of buffer after number checking
        if (count < checked + length)
            return false;
        
        checked += length;

        // now white spaces are allowed
        while 
        (
            count > checked
        && is_whitespace(buffer[offset + checked])
        )
            checked++;
                
        // validating for at least checked char space
        // to ensure that we are not out of buffer after whitespace checking
        if (count < checked)
            return false;
    }
    
    // STATUS: List<\w+>\s[\d\s]*\(
    //                          ^

    // validating for at least checked + 1 char space
    // to check for the brackets open "("
    if (count < checked + 1)
        return false;
    
    if (buffer[offset + checked + 1] != '(')
        return false;
    
    checked++;
    
    // STATUS: List<\w+>\s[\d\s]*\(
    //                            ^

    // from this point on, we are sure that this is a list
    return true;
}

int ListParser::parse_constant
(
    const char* buffer,
    int& offset,
    int& count
)
{
    int parsed = 0;

    // STATUS: List<\w+>\s[\d\s]*\(
    //         ^

    // validate a count of at least five
    // "List<" must be there
    if (count < 5)
        // TODO: return unexpected end of buffer
        return -1;
    
    parsed += 5;

    // STATUS: List<\w+>\s[\d\s]*\(
    //              ^
    
    // getting the list type
    int length = 0;
    while
    (
        count > parsed + length
     && is_letter(buffer[offset + 5 + length])
    )
        length++;
    
    // validate, that there is enough count
    if (count < parsed + length)
        // TODO: return unexpected end of buffer
        return -1;

    int blockyCount;
    std::string ltype
    (
        buffer + offset + parsed, 
        buffer + offset + parsed + length
    );

    if (ltype == "scalar")
        blockyCount = 1;
    else if (ltype == "vector")
        blockyCount = 3;
    else if (ltype == "tensor")
        blockyCount = 9;
    else
        // TODO: return meaningful codes
        return -1;
    
    parsed += length;

    // STATUS: List<\w+>\s[\d\s]*\(
    //                 ^

    if (count < parsed + 1)
        // TODO: return unexpected end of buffer
        return -1;
    
    parsed++;

    // STATUS: List<\w+>\s[\d\s]*\(
    //                  ^

    while
    (
        count > parsed
     && is_whitespace(buffer[offset + parsed])
    )
        parsed++;
    
    if (count < parsed)
        // TODO: return unexpected end of buffer
        return -1;

    // STATUS: List<\w+>\s[\d\s]*\(
    //                    ^

    if (is_number(buffer[count + checked]))
    {
        // numbers
        length = 1;
        while
        (
            count < parsed + length
        && is_number(buffer[offset + parsed + length])
        )
            length++;
        
        // validating for at least length + checked char space
        // to ensure, that we are not out of buffer after number checking
        if (count < parsed + length)
            // TODO: return unexpected end of buffer
            return -1;

        std::string amount
        (
            buffer + offset + parsed, 
            buffer + offset + parsed + length
        );

        data.resize_numbers((int)stod(amount.to_c()) * blockyCount);
        
        parsed += length;

        // whitespaces
        while 
        (
            count > parsed
        && is_whitespace(buffer[offset + parsed])
        )
            parsed++;
                
        // validating for at least checked char space
        // to ensure that we are not out of buffer after whitespace checking
        if (count < parsed)
            // TODO: return unexpected end of buffer
            return -1;
    }

    // STATUS: List<\w+>\s[\d\s]*\(
    //                          ^

    blockyParser = std::make_unique<BlockyParser>(data, blockyCount);

    return parsed;
}

int ListParser::parse_variable
(
    const char* buffer,
    int offset,
    int count
)
{
    if (count <= 0)
        // TODO: return expected end of buffer result
        return 1;

    int result = 1;
    int parsed = 0;
    while (result > 0)
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

            // if an error happened, forward it
            if (result < 0)
                return result;
            
            parsed += result;
        }
        else
        {
            // if not, checking whether it is an end of buffer or not
            if (count <= blockyParser->count())
                // TODO: return expected end of buffer
                return parsed;

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
