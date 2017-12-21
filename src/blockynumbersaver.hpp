#ifndef BLOCKYNUMBERSAVER_HPP
#define BLOCKYNUMBERSAVER_HPP

#include "blockynumber.hpp"

class BlockyNumberSaver
{
private:
    int index;
    BlockyNumber* values;
public:
    void initialize(int count);
    void write(BlockyNumber value);
};

#endif /* end of include guard: NUMBERSAVER_HPP */
