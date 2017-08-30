#include <parsing2/mainparser.hpp>

MainParser::MainParser(FILE* file)
    : total_parsed(0)
    , hooker(file, total_parsed)
    , parsers
    ({
        std::make_shared<ListParser>(hooker),
        std::make_shared<CharParser>(hooker)
    })
{

}

int MainParser::parse
(
    const char* buffer,
    int offset,
    int count
)
{
    int parsed = 0;

    int result = 0;

    bool someone_parsed = true;

    // checking, if parser, from another session wants to parse further
    if(curr_parser != nullptr)
    {
        // indicates, whether this is a fresh reload
        // important for parse variable, since you can determin,
        // if parse variable is finished
        // when parse variable is returning 0 at a fresh reload, then its finished
        bool fresh = true;

        result = 1;
        while (result != 0)
        {
            result = curr_parser->parse_variable
            (
                buffer,
                offset + parsed,
                count - parsed
            );

            if (result < 0)
            {
                std::cerr << "error happened while parsing variable" << '\n';
                std::cerr << "error code: " << result << '\n';
                exit(result);
            }

            total_parsed += result;
            parsed += result;

            // check, if parse variable is finished
            if (fresh && result == 0)
                curr_parser = nullptr;

            if (fresh)
                fresh = false;
        }

        // check, if parser is not finished
        if (curr_parser != nullptr)
            // if it is not
            // return, and parse again after reload
            return parsed;

        // if it is finished
        // continue with constant parsing
    }

    // while someone has parsed, continue to loop
    // if noone has parsed, than we need a reload
    while (someone_parsed)
    {
        // noone parsed in this round
        someone_parsed = false;
        for (auto parser : parsers)
        {
            // check, if parser can parse the given buffer
            if (parser->try_parse(buffer, offset + parsed, count - parsed))
            {
                // parse constant buffer
                result = parser->parse_constant
                (
                    buffer,
                    offset + parsed,
                    count - parsed
                );

                // check, if error happened
                if (result < 0)
                {
                    std::cerr << "error happened while parsing constant" << '\n';
                    std::cerr << "error code: " << result << '\n';
                    exit(result);
                }

                someone_parsed = true;

                // if no error happened
                // add up to parsed
                total_parsed += result;
                parsed += result;

                // parse variable buffer
                result = parser->parse_variable
                (
                    buffer,
                    offset + parsed,
                    count - parsed
                );

                // check, if parser_variable is implemented
                if (result == -404)
                    // if not, just continue
                    continue;

                // check, if error happened
                if (result < 0)
                {
                    std::cerr << "error happened while parsing variable" << '\n';
                    std::cerr << "error code: " << result << '\n';
                    exit(result);
                }

                // if it got here, then parse_variable is working
                // there is a sure return after this section of code
                // will continue after this session to ensure,
                // that the variable part is completely parsed

                // if result is 0, then buffer should be reloaded
                // => quiting
                if (result == 0)
                    return parsed;

                // frist, set the global parser to this parser
                curr_parser = parser;

                // add up to parsed
                total_parsed += result;
                parsed += result;

                // parser, while parse_variable returns 0
                while (result != 0)
                {
                    result = parser->parse_variable
                    (
                        buffer,
                        offset + parsed,
                        count - parsed
                    );

                    // check for error
                    if (result < 0)
                    {
                        std::cerr << "error while parsing variable" << '\n';
                        std::cerr << "error code: " << result << '\n';
                        exit(result);
                    }

                    total_parsed += result;
                    parsed += result;
                }

                // result is 0, so quiting
                return parsed;
            }
        }
    }

    return parsed;
}
