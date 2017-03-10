#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>

#include <blockynumber.hpp>

class Parser;

class Data
{
private:
    std::stringstream meta_data;
    std::vector<BlockyNumber> numbers;
    Parser* current_parser = NULL;

public:
    Data();

    void resize_numbers(int size)
    {
        numbers.resize(size);
    }

    void unset_current_parser()
    {
        current_parser = NULL;
    }

    void set_current_parser(Parser* parser)
    {
        current_parser = parser;
    }

};

#endif /* end of include guard: DATA_HPP */
