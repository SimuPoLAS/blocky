#include <iostream>
#include <fstream>

#include <io/blockyoutputstream.hpp>

int main(int argc, char const *argv[])
{
    BlockyOutputStream bky("resources/alpha.water.small.bky");
    ifstream file("resources/alpha.water.small");
    std::noskipws(file);

    uint8_t b;
    while(file >> b)
        bky << b;

    return 0;
}
