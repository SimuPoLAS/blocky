#include <iostream>
#include <fstream>

#include "io/blockyoutputstream.hpp"
#include "io/blockyinputstream.hpp"

int main(int argc, char const *argv[])
{
    BlockyOutputStream bky("resources/U.bky");
    ifstream file("resources/U", std::ios::binary);
    std::noskipws(file);

    uint8_t b;
    while(file >> b)
        bky << b;

	//BlockyInputStream bkyin("resources/alpha.water.bky");
	//ofstream filein("resources/alpha.water.bky.original");
	//std::noskipws(filein);

	//uint8_t bin;
	//while (bkyin >> bin)
	//	filein << bin;

    return 0;
}
