#ifndef BLOCKYNUMBERSAVER_HPP
#define BLOCKYNUMBERSAVER_HPP

#include <vector>

#include "blockynumber.hpp"

class BlockyNumberSaver
{
private:
    size_t size = 0;
    size_t index = 0;
public:
    std::vector<BlockyNumber> values;
    void initialize(size_t count);
    void write(BlockyNumber value);
    bool has_numbers();
    BlockyNumber get_next();
    void inc_index();
};

#endif /* end of include guard: NUMBERSAVER_HPP */
