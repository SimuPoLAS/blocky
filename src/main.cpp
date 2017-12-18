#include <iostream>
#include <fstream>

#include "io/blockyoutputstream.hpp"
#include "decomp/decompinputstream.hpp"

int main(int argc, char const *argv[])
{
    BlockyOutputStream bkyout("resources/alpha.water.bky");
    ifstream file("resources/alpha.water", std::ios::binary);
    std::noskipws(file);

    uint8_t b;
    while (file >> b)
        bkyout << b;

    bkyout.close();

    //return 0;

    DecompInputStream dcmpin("resources/alpha.water.bky");

    while (dcmpin >> b) {
        // do nothing
        // TODO: write to console
    }

    return 0;
}
