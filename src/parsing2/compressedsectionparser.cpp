#include "compressedsectionparser.hpp"

int CompressedsectionParser::try_parse
(
    const char* buffer,
    int offset,
    int count
)
{
    if (count < 8)
        return TRY_PARSE_BUFFER_SHORT;

    return TRY_PARSE_OK;
}

int CompressedsectionParser::parse
(
    const char* buffer,
    int offset,
    int count
)
{
    int processed = 0;
    // placeholder
    // TODO: real implementation

    // print buffer contents for testing purposes
    for (size_t i = 0; i < count; i++) {
        printf("%c", buffer[offset + i]);
        processed++;
    }
    printf("\n");

    return processed;
}