#include <string>
#include <string.h>

#include "decompstreambuffer.hpp"

using namespace std;

DecompStreamBuffer::DecompStreamBuffer()
    : opened(false)
    , last(false)
{
    setp(buffer, buffer + (bufferSize - 1));
    setg(buffer, buffer, buffer);
}

DecompStreamBuffer::~DecompStreamBuffer()
{
    close();
}

DecompStreamBuffer* DecompStreamBuffer::open(const char* name, int open_mode)
{
    if (is_open())
        return 0;

    mode = open_mode;

    if
    (
        (mode & std::ios::ate) ||
        (mode & std::ios::app) ||
        (
            (mode & std::ios::in) &&
            (mode & std::ios::out)
        )
    )
        return 0;

    char fmode[10];
    char* fmodeptr = fmode;

    if (mode & std::ios::in)
        *fmodeptr++ = 'r';
    if (mode & std::ios::out)
        *fmodeptr++ = 'w';
    *fmodeptr++ = 'b';
    *fmodeptr = '\0';


    // weird fixed length char arrays
    char data_name[1024];
    strcpy(data_name, name);
    strcat(data_name, ".data");
    char meta_name[1024];
    strcpy(meta_name, name);
    strcat(meta_name, ".meta");
    data = lzmaopen(data_name, fmode);
    meta = lzmaopen(meta_name, fmode);

    if (data == 0)
        return 0;

    opened = true;

    // make a distinction between reading and writing a file
    if (mode & std::ios::in) {
        //decompression = make_unique<DecompressionParser>(data, meta);
        printf("dcmpin triggered\n");
    } else {
        printf("dcmpout triggered\n");
    }

    return this;
}

DecompStreamBuffer* DecompStreamBuffer::close()
{
    if (is_open() && (mode & std::ios::out))
    {
        last = true;
        sync();
        opened = false;
        if (lzmaclose(data) == 0 && lzmaclose(meta) == 0)
            return this;
    }
    return 0;
}

int DecompStreamBuffer::underflow()
{
    //printf("underflow ocurred\n");
    if (!(mode & std::ios::in) || !opened)
        return EOF;

    //int num = lzmaread(buffer, 1, bufferSize, data);
    int num = bufferSize;

    //printf("num read %d", num);

    if (num <= 0)
        return EOF;

    /*
    for (size_t i = 0; i < num; i++) {
        printf("%c", buffer[i]);
    }
    */
    //int processed = decompression->parse(buffer, 0, num, last);
    int processed = num;

    // TODO: figure out a way to make the decompression parser say how much was
    // processed and set the get area pointer ACCORDINGLY
    setg(buffer, buffer, buffer + processed);

    //printf("\nreached underflow end\n");

    return *reinterpret_cast<unsigned char*>(gptr());
}

int DecompStreamBuffer::flush_buffer()
{
    int w = pptr() - pbase();

    int processed = w;

    // copy rest to beginning of buffer
    memcpy(buffer, buffer + processed, (bufferSize - processed));
    pbump(-processed);

    return processed;
}

int DecompStreamBuffer::overflow(int c)
{
    if (!(mode & std::ios::out) || !opened)
        return EOF;

    if (c != EOF)
    {
        *pptr() = c;
        pbump(1);
    }

    if (flush_buffer() == EOF)
        return EOF;

    return c;
}

int DecompStreamBuffer::sync()
{
    if (pptr() && pptr() > pbase())
    {
        if (flush_buffer() == EOF)
            return -1;
    }
    return 0;
}