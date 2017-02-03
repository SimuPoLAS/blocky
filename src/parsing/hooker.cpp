#include <parsing/hooker.hpp>

Hooker::Hooker(FILE* file)
    : file(file) { }

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
    if (type == ListType::Anonymous)
        return;
    if (inList)
        // TODO: throw meaningful exception, not just zero
        throw 0;
    inList = true;
    reporter = algorithm.compress(file, (int)type, capacity);
    size = uint8_t(type);
    if (reporter == nullptr)
        // TODO: throw meaningful exception, not just zero
        throw 0;
    start = PosProvider->position();
}

void Hooker::handle_list_entry(string value)
{
    // NOTE: this is the only source of BlockyNumbers
    // We are changing this to a shared_ptr since there is no way
    // to keep BlockyNumbers on stack when passing
    // They will be deallocated when leaving this function
    // Is also a big change because many function took a reference
    reporter->report
    (
        shared_ptr<BlockyNumber>
        (
            new BlockyNumber(BlockyNumber::parse(value))
        )
    );
}

void Hooker::handle_list_entries(string* values, size_t size)
{
    for (size_t i = 0; i < size; i++)
        reporter->report
        (
            shared_ptr<BlockyNumber>
            (
                new BlockyNumber(BlockyNumber::parse(values[i]))
            )
        );
}

void Hooker::leave_list()
{
    if (!inList)
        return;
    reporter->finish();
    CompessedDataSections.push_back
    (
        shared_ptr<CompressedSection>
        (
            new CompressedSection(start, PosProvider->position(), size)
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
