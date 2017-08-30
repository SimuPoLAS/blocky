#include <parsing2/charparser.hpp>

bool CharParser::try_parse
(
    const char* buffer,
    int offset,
    int count
)
{
    if (count < 1)
        return false;

    return true;
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
