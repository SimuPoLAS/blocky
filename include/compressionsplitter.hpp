#ifndef COMPRESSIONSPLITTER_HPP
#define COMPRESSIONSPLITTER_HPP

#include <memory>

#include <reporter.hpp>
#include <blockynumber.hpp>

using namespace std;

class CompressionSplitter : public Reporter
{
private:
    shared_ptr<shared_ptr<Reporter>> compressions;
    int32_t numberIndex;
    size_t count;

public:
    CompressionSplitter
    (
        shared_ptr<shared_ptr<Reporter>> compressions,
        size_t size
    );

    void report(shared_ptr<BlockyNumber> number) override;
    void report
    (
        // TODO: decide whether to use normal pointers
        // as parameter or shared_ptr
        shared_ptr<BlockyNumber>* numbers,
        size_t offset,
        size_t count
    ) override;
    void finish() override;
};

#endif /* end of include guard: COMPRESSIONSPLITTER_HPP */
