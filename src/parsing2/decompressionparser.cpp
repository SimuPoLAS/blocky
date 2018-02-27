#include "decompressionparser.hpp"

DecompressionParser::DecompressionParser(LZMAFILE* data, LZMAFILE* meta, std::vector<CompressedSection>& sections)
    : data(data), meta(meta), total(0), current_meta(0), current_data(0), ended(false), decompress(false), sections(sections), algorithm(), reader(data) { }

int DecompressionParser::fill_buffer(char* buffer, int buffer_size) {
    size_t processed = 0;

    // HOW IT'S SUPPOSED TO WORK:
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

        // HOW IT'S SUPPOSED TO WORK:
        // the general idea is, as you can't parse only parts of a compressed
        // section at will (blocky decompression isn't designed like that),
        // whenever we hit the DECOMPRESS PART, but have no numbers to fill the
        // buffer with, we decompress another section and save BlockyNumbers in
        // form of a BlockyNumberSaver, those numbers are then used to fill the
        // buffer until none are left anymore
        // we also pop sections we are done parsing so as to not accidentally
        // parse a section again, this entire process repeats until no sections
        // are left to parse

        // if the current_numbersave has no more numbers to be written to the buffer
        if (!current_numbersaver.has_numbers()) {
            // get new data
            // TODO: put in a lot of work to get this to function correctly

            // when no sections are left, we're pretty much EOF
            if (sections.size() == 0) {
                return -1;
            }

            // TODO: to truly replace the marerreader, have some sort of
            // reimplementation of number reporting like it is done here:
            // https://github.com/SimuPoLAS/Ofc/blob/master/src/Ofc/IO/MarerReader.cs#L131
            // also take special note that a variable size (as is used for
            // vectors and tensors, is yet to be implemented
            current_numbersaver = algorithm.decompress(data, buffer, reader, 1);
            sections.erase(sections.begin());
        }

        current_data = 0;
        while (current_numbersaver.has_numbers()) {
            // parse numbers to char and fill buffer
            int num;
            // TODO: figure out whether this is enough for a string repr of double values
            char buf[24];

            // reconstruct next number in numbersaver
            // result gets put into buf, number of characters gets put into num
            num = sprintf(buf, "%g", current_numbersaver.get_next().reconstructed());

            // if the buffer (+ newline char) has no more place for the current number
            if (current_data + num + 1 >= buffer_size) {
                break;
            }

            // copy from buf to buffer
            for (size_t i = 0; i < num; i++) {
                buffer[current_data + i] = buf[i];
            }
            buffer[current_data + num] = '\n';
            // inc num to account for extra newline char
            num++;

            // inc current_data
            current_data += num;
            // inc current_numbersaver index
            current_numbersaver.inc_index();

            // inc stat values
            total += num;
            processed += num;
        }
        // after filling the buffer with numbers, if the next iteration doesn't
        // require further filling with the current numbersaver, we set the
        // decompress flag to false to signal the processing of a META PART
        if (!current_numbersaver.has_numbers()) {
            decompress = false;
        }
        printf("fill_buffer processed %d\n", processed);
    } else {
        // this replaces JumpTo(section.START)
        printf("fill_buffer hit the META PART\n");
        printf("fill_buffer current_meta %d\n", current_meta);

        size_t start = sections.front().Start;
        size_t diff =  start - current_meta;

        // which is greater? the buffer size? or the amount of characters
        // we have yet to read? to_read gets assigned with the smaller one
        to_read = diff > buffer_size ? buffer_size : diff;
        printf("Section.START %d diff %d buffer size %d\n         => to_read   %d\n", start, diff, buffer_size, to_read);

        char c;
        int num;
        // for to_read characters
        for (int i = 0; i < to_read; i++) {
            // reads single character
            num = lzmaread(&c, sizeof(char), 1, meta);
            // no characters read means sudden end of stream
            if (num == 0) {
                printf("fill_buffer lzmaread not work, sudden end of stream\n");
                // TODO: error code shenanigans
                return -2;
            }
            // set char in buffer
            buffer[current_meta % buffer_size] = c;
            // inc stat values
            current_meta++;
            processed++;
        }
        // inc stat value
        total += to_read;

        printf("fill_buffer processed %d\n", processed);

        // if we're done with the whole section
        //printf("Section.START %d\n", sections.front().Start);
        if (current_meta == start) {
            decompress = true;
            // TODO: evaluate whether this needs to be reset or not after
            // a decompressed section
            current_meta = 0;
        }
    }

    printf("fill_buffer total     %d\n", total);
    return processed;
}
