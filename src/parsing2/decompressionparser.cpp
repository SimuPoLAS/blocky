#include "decompressionparser.hpp"

DecompressionParser::DecompressionParser(LZMAFILE* data, LZMAFILE* meta, std::vector<CompressedSection>& sections)
    : data(data), meta(meta), total(0), current_meta(0), current_data(0), ended(false), decompress(false), numbers_left(false), sections(sections), algorithm(), reader(data) { }

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
        // the general idea is, as you can't parse specific parts of a
        // compressed sections at will (blocky decompression isn't designed
        // like that), whenever the data_buffer is found empty, a new
        // BlockyDecompression is spawned that will generate some amount of
        // BlockyNumbers in form of a BlockyNumberSaver, and those numbers are
        // parsed into char arrays and put into the data_buffer until there are
        // no more numbers in the current BlockyNUmberSaver, the whole process
        // then repeats

        // if the current_numbersave has no more numbers to be written to the buffer
        if (!numbers_left) {
            // get new data
            // TODO: put in a lot of work to get this to function correctly

            // TODO: to truly replace the marerreader, have some sort of
            // reimplementation of number reporting like it is done here:
            // https://github.com/SimuPoLAS/Ofc/blob/master/src/Ofc/IO/MarerReader.cs#L131
            // also take special note that a variable size (as is used for
            // vectors and tensors, is yet to be implemented
            current_numbersaver = algorithm.decompress(data, buffer, reader, 1);

        // if data_buffer still has characters to be written to stream buffer
        } else {
            current_data = 0;
            while (current_numbersaver.has_numbers()) {
                // parse numbers to char and fill buffer
                int num;
                // TODO: figure out whether this is enough for a string repr of double values
                char buf[24];

                // reconstruct next number in numbersaver
                // result gets put into buf, number of characters gets put into num
                num = sprintf(buf, "%g", current_numbersaver.get_next().reconstructed());

                // if the buffer has no more place for the current number
                if (current_data + num >= buffer_size) {
                    break;
                }

                // copy from buf to buffer
                for (size_t i = 0; i < num; i++) {
                    buffer[current_data + i] = buf[i];
                }
                // inc current_data
                current_data += num;
                // inc current_numbersaver index
                current_numbersaver.inc_index();

                // inc stat values
                total += num;
                processed += num;
            }
            if (!current_numbersaver.has_numbers()) {
                decompress = false;
            }
            printf("fill_buffer processed %d\n", processed);
        }
    } else {
        // this replaces JumpTo(section.START)
        printf("fill_buffer hit the META PART\n");
        printf("fill_buffer current_meta %d\n", current_meta);

        size_t start = sections.front().Start;
        size_t diff =  start - current_meta;

        to_read = diff > buffer_size ? buffer_size : diff;
        printf("Section.START %d diff %d buffer size %d\n         => to_read   %d\n", start, diff, buffer_size, to_read);

        char c;
        int num;
        for (int i = 0; i < to_read; i++) {
            num = lzmaread(&c, sizeof(char), 1, meta);
            if (num == 0) {
                printf("fill_buffer lzmaread not work, sudden end of stream\n");
                return 0;
            }
            buffer[current_meta % buffer_size] = c;
            current_meta++;
            processed++;
        }
        total += to_read;

        printf("fill_buffer processed %d\n", processed);

        // if we're done with the whole section
        //printf("Section.START %d\n", sections.front().Start);
        if (current_meta == start) {
            decompress = true;
            current_meta = 0;
        }
    }

    printf("fill_buffer total     %d\n", total);
    return processed;
}
