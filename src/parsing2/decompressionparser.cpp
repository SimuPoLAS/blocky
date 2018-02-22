#include "decompressionparser.hpp"

DecompressionParser::DecompressionParser(LZMAFILE* data, LZMAFILE* meta, std::vector<CompressedSection>& sections)
    : data(data), meta(meta), total(0), current_meta(0), current_data(0), ended(false), decompress(false), sections(sections), algorithm(), reader(data) { }

int DecompressionParser::fill_buffer(char* buffer, int buffer_size) {
    size_t processed = 0;
    printf("fill_buffer current_meta   %d\n", current_meta);

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

        sections.erase(sections.begin());
        decompress = false;
        buffer[0] = 'a';
        return ++processed;

        // HOW IT'S SUPPOSED TO WORK:
        // the general idea is, as you can't parse specific parts of a
        // compressed sections at will (blocky decompression isn't designed
        // like that), whenever the data_buffer is found empty, a new
        // BlockyDecompression is spawned that will generate some amount of
        // BlockyNumbers in form of a BlockyNumberSaver, and those numbers are
        // parsed into char arrays and put into the data_buffer until there are
        // no more numbers in the current BlockyNUmberSaver, the whole process
        // then repeats

        printf("fill_buffer total     %d\n", total);

        size_t data_buffer_size = data_buffer.size();
        // if data_buffer has been fully written to our stream buffer
        if (current_data == data_buffer_size) {
            // get new data
            // TODO: put in a lot of work to get this to function correctly

            // TODO: to truly replace the marerreader, have some sort of
            // reimplementation of number reporting like it is done here:
            // https://github.com/SimuPoLAS/Ofc/blob/master/src/Ofc/IO/MarerReader.cs#L131
            // also take special note that a variable size (as is used for
            // vectors and tensors, is yet to be implemented
            current_numbersaver = algorithm.decompress(data, buffer, reader, 1);

            // parse numbers to char to fill data_buffer
            int num;
            // TODO: figure out whether this is enough for a string repr of double values
            char buf[24];
            for (size_t i = 0; i < current_numbersaver.values.size(); i++) {
                num = sprintf(buf, "%g", current_numbersaver.values[i].reconstructed());
                for (size_t j = 0; j < num; j++) {
                    data_buffer.push_back(buf[j]);
                }
            }

            current_data = 0;
        // if data_buffer still has characters to be written to stream buffer
        } else {
            size_t diff = data_buffer_size - current_data;
            to_read = diff > buffer_size ? buffer_size : diff;

            for (int i = 0; i < to_read; i++) {
                buffer[i] = data_buffer[i];
                current_data++;
            }
            total += to_read;

            printf("fill_buffer processed %d\n", processed);
        }

        decompress = false;
    } else {
        // this replaces JumpTo(section.START)
        printf("fill_buffer hit the META PART\n");

        size_t start = sections.front().Start;
        size_t diff =  start - current_meta;

        to_read = diff > buffer_size ? buffer_size : diff;
        printf("Section.START %d diff %d buffer size %d => to_read %d\n", start, diff, buffer_size, to_read);

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
