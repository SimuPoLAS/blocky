#include "compressionsplitter.hpp"

CompressionSplitter::CompressionSplitter
(
    std::vector<shared_ptr<Reporter>> compressions,
    size_t size
)
    : compressions(compressions)
    , count(size) 
	, numberIndex(0) { }

void CompressionSplitter::report(BlockyNumber number)
{
    compressions[numberIndex]->report(number);
    numberIndex = (numberIndex + 1) % count;
}

void CompressionSplitter::report
(
    // TODO: decide whether to use normal pointers
    // as parameter or shared_ptr
    BlockyNumber* numbers,
    size_t offset,
    size_t count
)
{
    for (size_t i = offset; i < offset + count; i++)
        report(numbers[i]);
}

void CompressionSplitter::finish()
{
    for (size_t i = 0; i < count; i++)
    {
        compressions[i]->finish();
    }
}
