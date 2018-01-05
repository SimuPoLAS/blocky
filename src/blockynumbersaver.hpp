#ifndef BLOCKYNUMBERSAVER_HPP
#define BLOCKYNUMBERSAVER_HPP

#include <vector>

#include "blockynumber.hpp"

class BlockyNumberSaver
{
private:
    int index = 0;
public:
    std::vector<BlockyNumber> values;
    void initialize(int count);
    void write(BlockyNumber value);
};

#endif /* end of include guard: NUMBERSAVER_HPP */
