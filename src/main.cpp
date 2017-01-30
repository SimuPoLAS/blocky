#include <iostream>
#include <fstream>

#include <blockyoutputstream.hpp>

int main(int argc, char const *argv[])
{
    BlockyOutputStream bky("resources/alpha.water.bky");
    ifstream file("resources/alpha.water");
    std::noskipws(file);

    uint8_t b;
    while(file >> b)
        bky << b;

    return 0;
}
