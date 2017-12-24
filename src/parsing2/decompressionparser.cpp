#include "decompressionparser.hpp"

DecompressionParser::DecompressionParser(LZMAFILE* data, LZMAFILE* meta)
    : data(data), meta(meta), current(0), ended(false), decompress(false)
{}

int DecompressionParser::fill_buffer(char* buffer, int bufferSize, std::vector<CompressedSection> &sections) {
    size_t processed = 0;

    // so the idea is, we want to read the contents of metadata UNTIL we hit
    // the start of a compressed section, we immediately return and next time
    // the buffer has to be filled, we decompress

    // the parsing of one type of file goes on until the type of file has
    // to to be switched, this makes it easy to work with data and metadata
    // at the same time
    if (decompress) {
        // this replaces the rest of MarerReader
        // (the part responsible for decompressing)

        // debug placeholders
        printf("fill_buffer hit the DECOMPRESS PART\n");
        decompress = false;
    } else {
        // this replaces JumpTo(section.START)
        size_t diff = sections.front().Start - current;

        to_read = diff > (bufferSize - current) ? (bufferSize - current) : diff;

        char c;
        int num;
        for (int i = 0; i < to_read; i++) {
            num = lzmaread(&c, sizeof(char), 1, meta);
            if (num == 0) {
                printf("fill_buffer lzmaread not work, sudden end of stream\n");
                return 0;
            }
            buffer[current] = c;
            current++;
            processed++;
        }

        printf("fill_buffer hit the META PART\n");
        printf("fill_buffer processed %d\n", processed);

        // if we're done with the whole section
        if (current == sections.front().Start) {
            decompress = true;
            current = 0;
        }
    }

    return processed;
}
