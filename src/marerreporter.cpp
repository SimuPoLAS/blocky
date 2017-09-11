#include "marerreporter.hpp"

MarerReporter::MarerReporter(BitWriter & writer) : writer(writer)
{
	set_size(1);
}

size_t MarerReporter::get_size()
{
	return buffer.size();
}

void MarerReporter::set_size(size_t size)
{
	buffer.resize(size);
	position = 0;
}

void MarerReporter::report(BlockyNumber value)
{
	buffer[position++] = value.to_s();
	if (position == buffer.size())
	{
		// so to get things clear, this method is actually responsible
		// for the conversion of our data (in form of blockynumbers) to
		// strings that are written directly to the output file

		// please don't implement rudimentary code before actually checking
		// how this usually gets implemented in cpp projects
		
		// for future reference: 
		// https://github.com/SimuPoLAS/Ofc/blob/master/src/Ofc/IO/MarerReader.cs#L131
		switch (get_size())
		{
		case 1:
			// TODO: check how to actually use write
			// ofc: _writer.WriteLine(_buffer[0]);
			//writer.write(buffer[0],)
		default:
			break;
		}
		position = 0;
	}
}

void MarerReporter::report(BlockyNumber* values, size_t offset, size_t amount)
{
	for (size_t i = offset; i < offset + amount; i++)
		report(values[i]);
}

void MarerReporter::finish()
{
	// do nothing and be useless, just like in ofc :)
}
