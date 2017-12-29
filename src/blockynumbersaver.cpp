#include "blockynumbersaver.hpp"

void BlockyNumberSaver::initialize(int count) {
    values.resize(count);
}

void BlockyNumberSaver::write(BlockyNumber value) {
    values[index] = value;
    index++;
}