#include <iostream>

#include "hooker.hpp"

Hooker::Hooker(FILE* data, FILE* meta, uint32_t const& providedPosition)
    : algorithm()
	, data(data)
	, meta(meta)
	, meta_str("")
	, inList(false)
	, type(ListType::Anonymous)
	, start(0)
	, size(0)
    , providedPosition(providedPosition) { }

void Hooker::handle_meta_char(char c)
{
	// TODO: handle meta char and later, compress to as lzma into meta file
	fwrite(&c, sizeof(char), 1, meta);
}

void Hooker::handle_meta_char_array(const char* c, size_t size)
{
	// TODO: benchmark if this is performancewise acceptable
	// we could instead write the whole array into the file
	// with fwrite
	for (size_t i = 0; i < size; i++)
		handle_meta_char(c[i]);
}

void Hooker::enter_dictionary(string name)
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::leave_dictionary()
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::enter_code_stream_dictionary(string name)
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::leave_code_stream_dictionary()
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::enter_entry(string name)
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::leave_entry()
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::enter_list(ListType type, int capacity)
{
    std::cout << "entered list" << '\n';
    if (type == ListType::Anonymous)
        return;
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
    this->type = type;
    inList = true;
    reporter = algorithm.compress(data, (int)type, capacity);
    size = uint8_t(type);
    if (reporter == nullptr)
        // TODO: throw meaningful exception, not just zero
        throw 0;
    start = providedPosition;
}

void Hooker::handle_list_entry(string value)
{
    // NOTE: this is the only source of BlockyNumbers
    // We are changing this to a shared_ptr since there is no way
    // to keep BlockyNumbers on stack when passing
    // They will be deallocated when leaving this function
    // Is also a big change because many function took a reference
    reporter->report(BlockyNumber::parse(value));
}

void Hooker::handle_list_entries(string* values, size_t size)
{
    for (size_t i = 0; i < size; i++)
        reporter->report(BlockyNumber::parse(values[i]));
}

void Hooker::leave_list()
{
    if (!inList)
        return;
    std::cout << "finished list" << '\n';
    reporter->finish();
    CompessedDataSections.push_back
    (
        shared_ptr<CompressedSection>
        (
            new CompressedSection(start, providedPosition, size)
        )
    );
    inList = false;
}

void Hooker::handle_macro(DirectiveType directive, string data)
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::handle_dimension(string* values, size_t size)
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::handle_scalar(string value)
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::handle_keyword(string value)
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::handle_string(string data)
{
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
}

void Hooker::flush() { }