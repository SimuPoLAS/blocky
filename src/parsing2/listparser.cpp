#include <cctype>

#include "listparser.hpp"

bool ListParser::try_parse
(
    const char* buffer,
    int offset,
    int count
)
{
    // REGEX: List<\w+>\s[\d\s]*\(\s
    int checked = 0;

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //         ^

    // validating for at least 4 char space
    // to check for the "List" keyword
    if (count < checked + 4)
        return false;

    // making a string from the first 4 chars in buffer
    std::string keyword(buffer+offset, buffer+offset+4);

    // validating, if the string equals "List"
    if (keyword != "List")
        return false;

    // 4 chars were validated
    checked += 4;

    // STATUS: List<\w+>\s[\d\s]*\(\s
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

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //              ^

    // declaring a length for the keyword between the chevrons
    int length = 0;

    // determining the length of the string between the chevrons
    while
    (
        count > checked + length
     && isalpha(buffer[offset + checked + length])
    )
        length++;

    // validating for at least length + 1 char space
    // to check for the List Type keyword and chevrons close ">"
    if (count < checked + length + 1)
        return false;

    // validating, if at the and of the keyword, there is a chenvrons close
    if (buffer[offset + checked + length] != '>')
        return false;

    // saving the List Type in a string
    std::string ltype
    (
        buffer + offset + checked,
        buffer + offset + checked + length
    );

	// length chars were validated
	checked += length + 1;

	// STATUS: List<\w+>\s[\d\s]*\(\s
	//                  ^

    // validating, if List Type is supported
    if (ltype != "scalar" && ltype != "vector" && ltype != "tensor")
        return false;

    // now white spaces are allowed
    while
    (
        count > checked
     && isspace(buffer[offset + checked])
    )
        checked++;

    // validating for at least checked char space
    // to ensure that we are not out of buffer after whitespace checking
    if (count < checked)
        return false;

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //                    ^

    // now there may be a number (with whitespaces after it)
    if (isdigit(buffer[offset + checked]))
    {
        length = 1;
        while
        (
            count > checked + length
         && isdigit(buffer[offset + checked + length])
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
         && isspace(buffer[offset + checked])
        )
            checked++;

        // validating for at least checked char space
        // to ensure that we are not out of buffer after whitespace checking
        if (count < checked)
            return false;
    }

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //                            ^

    if (buffer[offset + checked] != '(')
        return false;

    checked++;

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //                             ^

    while
	(
		count > checked
		&& isspace(buffer[offset + checked])
	)
		checked++;

    // from this point on, we are sure that this is a list
    return true;
}

int ListParser::parse_constant
(
    const char* buffer,
    int offset,
    int count
)
{
    int parsed = 0;

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //         ^

    // validate a count of at least five
    // "List<" must be there
    if (count < 5)
        // TODO: return unexpected end of buffer
        return -1;

    parsed += 5;

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //              ^

    // getting the list type
    int length = 0;
    while
    (
        count > parsed + length
     && isalpha(buffer[offset + 5 + length])
    )
        length++;

    // validate, that there is enough count
    if (count < parsed + length)
        // TODO: return unexpected end of buffer
        return -1;

    ListType type;
    std::string ltype
    (
        buffer + offset + parsed,
        buffer + offset + parsed + length
    );

    if (ltype == "scalar")
        type = ListType::Scalar;
    else if (ltype == "vector")
        type = ListType::Vector;
    else if (ltype == "tensor")
        type = ListType::Tensor;
    else
        // TODO: return meaningful codes
        return -1;

    parsed += length;

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //                 ^

    if (count < parsed + 1)
        // TODO: return unexpected end of buffer
        return -1;

    parsed++;

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //                  ^

    while
    (
        count > parsed
     && isspace(buffer[offset + parsed])
    )
        parsed++;

    if (count < parsed)
        // TODO: return unexpected end of buffer
        return -1;

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //                    ^

    int amount = (int)type;
    if (isdigit(buffer[offset + parsed]))
    {
        // numbers
        length = 1;
        while
        (
            count > parsed + length
        && isdigit(buffer[offset + parsed + length])
        )
            length++;

        // validating for at least length + checked char space
        // to ensure, that we are not out of buffer after number checking
        if (count < parsed + length)
            // TODO: return unexpected end of buffer
            return -1;

        std::string amount_str
        (
            buffer + offset + parsed,
            buffer + offset + parsed + length
        );

        amount = stoi(amount_str.c_str()) * (int)type;

        parsed += length;

        // whitespaces
        while
        (
            count > parsed
        && isspace(buffer[offset + parsed])
        )
            parsed++;

        // validating for at least checked char space
        // to ensure that we are not out of buffer after whitespace checking
        if (count < parsed)
            // TODO: return unexpected end of buffer
            return -1;
    }

    // STATUS: List<\w+>\s[\d\s]*\(\s
    //                          ^

    hooker.enter_list(type, amount);

    blockyParser = std::make_unique<BlockyParser>(hooker);

    end = false;

	// parse (
	parsed++;

	// parse whitespaces
	while
	(
		count > parsed
		&& isspace(buffer[offset + parsed])
	)
		parsed++;

    return parsed;
}

int ListParser::parse_variable
(
    const char* buffer,
    int offset,
    int count
)
{
    if (end)
        return 0;

	// return 0 if there is nothing to parse
	if (count == 0)
		return 0;

    int result = 1;
    int parsed = 0;
    while (result > 0)
    {
        // trying to parse the variable record
        if (blockyParser->try_parse(buffer, offset + parsed, count - parsed))
        {
            // if it is parsable, then parse
            result = blockyParser->parse_constant
            (
                buffer,
                offset + parsed,
                count - parsed
            );

            // if an error happened, forward it
            if (result < 0)
                return result;

            parsed += result;
        }
        else
        {
            // if not, returning what we parsed
            return parsed;
        }

        // after parsing is done, we check for the escape sequence
        if (buffer[offset + parsed] == ')')
        {
            // set the end of parsing
			hooker.leave_list();
            end = true;
            return parsed;
        }
    }

    return parsed;
}
