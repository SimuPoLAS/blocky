#include "decompressionparser.hpp"

DecompressionParser::DecompressionParser(LZMAFILE* data, LZMAFILE* meta)
    : data(data)
    , meta(meta)
    , total_parsed(0)
    , parsers
    ({
        std::make_shared<CompressedsectionParser>()
    })
    , curr_parser(parsers[0])
{

}

int DecompressionParser::parse
(
    const char* buffer,
    int offset,
    int count,
    bool last
)
{
    int parsed = 0;

    int result = 0;

    if (curr_parser != nullptr) {
        int try_parse_result = curr_parser->try_parse
        (
            buffer,
            offset + parsed,
            count - parsed
        );

        printf("try parse result %d", try_parse_result);

        if (try_parse_result == TRY_PARSE_OK) {
            // if the current parser indicates it's done, move on to the next
            // NOTE: apparently unnecessary, as Ofc only parses compressedsections
            /*
            if (result == PARSER_FINISHED) {
                curr_parser = ++curr_parser;
            }
            */

            // if some error happens, forward the error
            if (result < 0) {
                return result;
            }

            result = 1;
            while (result != 0) {
                printf("yaaay parsing");
                result = curr_parser->parse
                (
                    buffer,
                    offset + parsed,
                    count - parsed
                );

                total_parsed += result;
                parsed += result;
            }
        }
    }

    return parsed;
}

void DecompressionParser::end()
{
}
