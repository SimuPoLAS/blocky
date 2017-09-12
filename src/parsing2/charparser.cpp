#include "charparser.hpp"

int CharParser::try_parse
(
    const char* buffer,
    int offset,
    int count
)
{
    if (count < 1)
        return TRY_PARSE_BUFFER_SHORT;

    return TRY_PARSE_OK;
}

int CharParser::parse_constant
(
    const char* buffer,
    int offset,
    int count
)
{
    // TODO: add char to hooker for meta compression (LZMA)
    return 1;
}
