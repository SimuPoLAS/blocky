#ifndef MAINPARSER_HPP
#define MAINPARSER_HPP

#include "parser.hpp"
#include "listparser.hpp"
#include "charparser.hpp"
#include "../parsing/hooker.hpp"

class MainParser
{
private:
    int total_parsed = 0;
    Hooker hooker;
    std::vector<std::shared_ptr<Parser>> parsers;

    std::shared_ptr<Parser> curr_parser = nullptr;

public:
    MainParser(FILE* data, FILE* meta);

    int parse
    (
        const char* buffer,
        int offset,
        int count
    );

	void end();
};

#endif /* end of include guard: MAINPARSER_HPP */
