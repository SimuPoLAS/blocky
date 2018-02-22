#include "blockynumbersaver.hpp"

void BlockyNumberSaver::initialize(size_t count) {
    values.resize(count);
}

void BlockyNumberSaver::write(BlockyNumber value) {
    values[size] = value;
    size++;
}

bool BlockyNumberSaver::has_numbers() {
    if (index >= size) {
        return false;
    }
    return true;
}

BlockyNumber BlockyNumberSaver::get_next() {
    return values[index];
}

void BlockyNumberSaver::inc_index() {
    index++;
}
