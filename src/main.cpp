#include <iostream>
#include <fstream>

#include "io/blockyoutputstream.hpp"
#include "io/blockyinputstream.hpp"

int main(int argc, char const *argv[])
{
    BlockyOutputStream bkyout("resources/U.small.bky");
    ifstream file("resources/U.small", std::ios::binary);
    std::noskipws(file);

    uint8_t b;
    while (file >> b)
        bkyout << b;

    // just testin
    BlockyInputStream bkyin("resources/U.small.bky");
    while (bkyin >> b) {
        // do nothing
    }

    return 0;
}
