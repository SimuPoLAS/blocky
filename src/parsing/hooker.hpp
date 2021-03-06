#ifndef HOOKER_HPP
#define HOOKER_HPP

#include <memory>

#include "listtype.hpp"
#include "directivetype.hpp"
#include "../lzmaio.hpp"
#include "../reporter.hpp"
#include "../blockyalgorithm.hpp"
#include "../blockynumber.hpp"
#include "../compressedsection.hpp"

using namespace std;

class Hooker {
private:
    BlockyAlgorithm algorithm;
	LZMAFILE* data;
	LZMAFILE* meta;
    shared_ptr<Reporter> reporter;
    bool inList;
    ListType type;
    uint32_t start;
    uint8_t size;
	std::string meta_str;

public:
	size_t& providedPosition;
    vector<shared_ptr<CompressedSection>> CompessedDataSections;

    Hooker(LZMAFILE* data, LZMAFILE* meta, size_t& providedPosition);

    bool is_in_list() const { return inList; }
    ListType get_type() const { return type; }

	void handle_meta_char(char c);
	void handle_meta_char_array(const char* c, size_t size);
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
	void end_file();
    void flush();
};

#endif /* end of include guard: HOOKER_HPP */
