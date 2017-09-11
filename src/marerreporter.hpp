#ifndef MARERREPORTER_HPP
#define MARERREPORTER_HPP

#include <vector>
#include <string>

#include "bitwriter.hpp"
#include "blockynumber.hpp"
#include "reporter.hpp"

class MarerReporter
	: public Reporter
{
private:

	std::vector<std::string> buffer;
	size_t position;
	BitWriter& writer;

	size_t get_size();
	void set_size(size_t size);
public:
	MarerReporter(BitWriter& writer);

	void report(BlockyNumber value);
	void report(std::vector<BlockyNumber> values, size_t offset, size_t amount);
};

#endif /* end of include guard: MARERREPORTER_HPP */