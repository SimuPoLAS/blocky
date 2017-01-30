#include <string>
#include <string.h>

#include <blockystreambuffer.hpp>

using namespace std;

BlockyStreamBuffer::BlockyStreamBuffer()
    : opened(false)
{
    setp(buffer, buffer + (bufferSize - 1));
    setg(buffer, buffer, buffer);
}

BlockyStreamBuffer::~BlockyStreamBuffer()
{
    close();
}

BlockyStreamBuffer* BlockyStreamBuffer::open(const char* name, int open_mode)
{
    std::cout << "Buffer will be opened" << '\n';

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

    file = fopen(name, fmode);

    if (file == 0)
        return 0;

    opened = true;

    lexer = unique_ptr<Lexer>(new Lexer(buffer));

    return this;
}

BlockyStreamBuffer* BlockyStreamBuffer::close()
{
    std::cout << "buffer will be closed" << '\n';

    if (is_open())
    {
        sync();
        opened = false;
        if (fclose(file) == 0)
            return this;
    }
    return 0;
}

int BlockyStreamBuffer::underflow()
{
    std::cout << "underflow" << '\n';
    if (!(mode & std::ios::in) || !opened)
        return EOF;

    int num = fread(buffer, 1, bufferSize, file);

    if (num <= 0)
        return EOF;

    setg(buffer, buffer, buffer + num);

    return *reinterpret_cast<unsigned char*>(gptr());
}

int BlockyStreamBuffer::flush_buffer()
{
    std::cout << "flushing buffer" << '\n';
    int w = pptr() - pbase();

    Token* tokens[4096];

    std::cout << "lexer read" << '\n';
    int processed = lexer->read(tokens, 4096, w);

    // if (fwrite(pbase(), 1, w, file) == EOF)
    //     return EOF;

    // copy rest to beginning of buffer
    memcpy(buffer, buffer + processed, (bufferSize - processed));
    pbump(-processed);

    return processed;
}

int BlockyStreamBuffer::overflow(int c)
{
    std::cout << "overflow" << '\n';
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

int BlockyStreamBuffer::sync()
{
    std::cout << "sync" << '\n';
    if (pptr() && pptr() > pbase())
    {
        if (flush_buffer() == EOF)
            return -1;
    }
    return 0;
}

// Token next_token()
// {
//     char c;
//     while (needs(1))
//     {
//         // get char
//         if (!eat(c))
//             return processed;
//
//         // continue while c is a whitespace
//         while(isspace(c))
//             if (!eat(c))
//                 return processed;
//
//         // everything until now is ready to be dumped
//         processed = position;
//
//         // checking for comment
//         if (c == '/')
//         {
//             if (!eat(c))
//                 return processed;
//
//             if (c == '/')
//             {
//                 if (!skip_until('\n'))
//                     return processed;
//                 processed = position;
//                 continue;
//             }
//             else if (c == '*')
//             {
//                 if (!skip_until("*/", 2))
//                     return processed;
//                 processed = position;
//                 continue;
//             }
//         }
//
//         // if it gets to here
//         // we can ensure, that there are no comments
//         // and whitespaces
//         // => there must be a token!
//         // so we set the current position as starting position for token
//         tokenStartPosition = position;
//
//         // checking for string containers #{.*?}#
//         if (c == '#')
//         {
//             if (!eat(c))
//                 return processed;
//
//             if (c == '{')
//             {
//                 string s;
//                 if (!eat_until("}#", 2, s))
//                     return processed;
//                 processed = position;
//                 return
//             }
//         }
//
//         // checking if current char is token
//     }
// }
//
// int BlockyStreamBuffer::skip(int amount)
// {
//     char c;
//     if (!eat(c))
//         return -1;
//     return c;
// }
//
// int BlockyStreamBuffer::skip_until(char escape)
// {
//     char c;
//     if (!eat(c))
//         return -1;
//
//     while (c != escape)
//         if (!eat(c))
//             return -1;
//     return c;
// }
//
// int BlockyStreamBuffer::skip_until(char const* escapes, int size)
// {
//     char c;
//     if (!eat(c))
//         return -1;
//
//     for (size_t i = 0; i < size; i++) {
//         char escape = escapes[i];
//
//         if (i > 0)
//         {
//             if (!eat(c))
//                 return -1;
//
//             if (c == escape)
//                 continue
//             else
//             {
//                 i = -1;
//                 continue;
//             }
//         }
//
//         while (c != escape)
//             if (!eat(c))
//                 return -1;
//     }
//
//     return c;
// }
//
// int BlockyStreamBuffer::eat(char& c)
// {
//     if (!needs(1))
//     {
//         pbump(-processed);
//         return -1;
//     }
//     c = buffer[position];
//     position++;
//     return c;
// }
//
// int BlockyStreamBuffer::eat_until(char const* escape, int size, string& s)
// {
//     uint32_t start = position;
//     uint32_t length = 0;
//     char c;
//
//     if (!eat(c))
//         return -1;
//
//     while(c != escape)
//     {
//         lenght++;
//         if (!eat(c))
//             return -1;
//     }
//
//     s((char*)(*buffer + start), length);
//     return c;
// }
//
// int BlockyStreamBuffer::eat_until(char const* escapes, int size, string&)
// {
//     uint32_t start = position;
//     uint32_t length = 0;
//     char c;
//
//     if (!eat(c))
//         return -1;
//
//     for (size_t i = 0; i < size; i++)
//     {
//         char escape = escapes[i];
//
//         if (i > 0)
//         {
//             if (!eat(c))
//                 return -1;
//
//             if (c == escape)
//                 continue;
//             else
//             {
//                 i = -1;
//                 continue;
//             }
//         }
//
//         while (c != escape)
//         {
//             length++;
//             if (!eat(c))
//                 return -1;
//         }
//     }
//
//     s((char*)(*buffer + start), length);
//     return c;
// }
//
// bool BlockyStreamBuffer::needs(int amount)
// {
//     return (width - (position-1) >= amount)
// }
