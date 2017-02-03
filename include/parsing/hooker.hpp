#ifndef HOOKER_HPP
#define HOOKER_HPP

#include <memory>

#include <parsing/positionprovider.hpp>
#include <parsing/listtype.hpp>
#include <parsing/directivetype.hpp>
#include <reporter.hpp>
#include <blockyalgorithm.hpp>
#include <blockynumber.hpp>
#include <compressedsection.hpp>

using namespace std;

class Hooker {
private:
    BlockyAlgorithm algorithm;
    FILE* file;
    shared_ptr<Reporter> reporter;
    bool inList;
    uint32_t start;
    uint8_t size;

public:
    vector<shared_ptr<CompressedSection>> CompessedDataSections;
    shared_ptr<PositionProvider> PosProvider;

    Hooker(FILE* file);

    void enter_dictionary(string name);
    void leave_dictionary();
    void enter_code_stream_dictionary(string name);
    void leave_code_stream_dictionary();
    void enter_entry(string name);
    void leave_entry();
    void enter_list(ListType type, int capacity);
    void handle_list_entry(string value);
    void handle_list_entries(string* values, size_t size);
    void leave_list();
    void handle_macro(DirectiveType directive, string data);
    void handle_dimension(string* values, size_t size);
    void handle_scalar(string value);
    void handle_keyword(string value);
    void handle_string(string data);
    void flush();
};

#endif /* end of include guard: HOOKER_HPP */
