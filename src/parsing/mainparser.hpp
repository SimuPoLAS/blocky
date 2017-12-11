#ifndef MAINPARSER_HPP
#define MAINPARSER_HPP

#include "parser.hpp"
#include "listparser.hpp"
#include "charparser.hpp"
#include "../lzmaio.hpp"
#include "../parsing/hooker.hpp"

class MainParser
{
private:
    size_t total_parsed = 0;
    Hooker hooker;
    std::vector<std::shared_ptr<Parser>> parsers;

    std::shared_ptr<Parser> curr_parser = nullptr;

public:
    MainParser(LZMAFILE* data, LZMAFILE* meta);

    int parse
    (
        const char* buffer,
        int offset,
        int count,
		bool last
    );

	void end();
};

#endif /* end of include guard: MAINPARSER_HPP */
