#include "mainparser.hpp"

MainParser::MainParser(LZMAFILE* data, LZMAFILE* meta)
    : total_parsed(0)
    , hooker(data, meta, total_parsed)
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
    int count,
	bool last
)
{
    int parsed = 0;

    int result = 0;

    bool someone_parsed = true;

    // checking, if parser, from another session wants to parse further
    if(curr_parser != nullptr)
    {
        // indicates, whether this is a fresh reload
        // important for parse variable, since you can determine,
        // if parse variable is finished
        // (the parse variable returns 0 at a fresh reload when it is finished)
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
    // if noone has parsed, then we need a reload
    while (someone_parsed)
    {
        // no one parsed in this iteration
        someone_parsed = false;
        for (auto parser : parsers)
        {
			int try_parse_result = parser->try_parse(buffer, offset + parsed, count - parsed);
            // check, if parser can parse the given buffer
			if (try_parse_result == TRY_PARSE_OK)
			{
				// parse constant buffer
				result = parser->parse_constant
				(
					buffer,
					offset + parsed,
					count - parsed
				);

				// check for errors (constant parsing)
				if (result < 0)
					return result;

				someone_parsed = true;

				// if there were no errors
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

				// check for errors (variable parsing)
				if (result < 0)
				{
					std::cerr << "error happened while parsing variable" << '\n';
					std::cerr << "error code: " << result << '\n';
					exit(result);
				}

				// if the code reached here, parse_variable is working
				// and there is a sure return after this section of code
				// will continue after this session to ensure,
				// that the variable part is completely parsed

				// first, set the global parser to this parser
				curr_parser = parser;

				// if result is 0, then buffer should be reloaded
				// => quit and refill buffer
				if (result == 0)
					return parsed;

				// add up to parsed
				total_parsed += result;
				parsed += result;

				// parse, as long as parse_variable returns 0
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

				// result is 0
				// => quit and refill buffer
				return parsed;
			}
			else if (try_parse_result == TRY_PARSE_BUFFER_SHORT && !last)
			{
				// if the try_parse method indicates, that the buffer needs more place
				// then return to trigger buffer flushing
				return parsed;
			}
        }
    }

    return parsed;
}

void MainParser::end()
{
	hooker.end_file();
}
