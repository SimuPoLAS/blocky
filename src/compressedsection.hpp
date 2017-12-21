#ifndef COMPRESSEDSECTION_HPP
#define COMPRESSEDSECTION_HPP

struct CompressedSection
{
    size_t Start;
    size_t End;
    uint8_t Size;

    CompressedSection(uint32_t start, uint32_t end, uint8_t size)
        : Start(start)
        , End(end)
        , Size(size) { }
};

#endif /* end of include guard: COMPRESSEDSECTION_HPP */
