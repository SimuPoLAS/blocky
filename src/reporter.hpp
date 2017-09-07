#ifndef REPORTER_HPP
#define REPORTER_HPP

#include <blockynumber.hpp>

class Reporter
{
public:
    void virtual report(BlockyNumber number) = 0;
    void virtual report
    (
        // TODO: decide whether to use normal pointers
        // as parameter or shared_ptr
        BlockyNumber* numbers,
        size_t offset,
        size_t count
    ) = 0;
    void virtual finish() = 0;
};

#endif /* end of include guard: REPORTER_HPP */
