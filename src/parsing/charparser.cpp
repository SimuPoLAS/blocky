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

    return 1;
}

int CharParser::parse_constant
(
    const char* buffer,
    int offset,
    int count
)
{
	hooker.handle_meta_char(buffer[offset]);
    return 1;
}
