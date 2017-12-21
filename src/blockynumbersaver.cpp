#include "blockynumbersaver.hpp"

void BlockyNumberSaver::initialize(int count) {
    values = new BlockyNumber[count];
}

void BlockyNumberSaver::write(BlockyNumber value) {
    values[index] = value;
    index++;
}
