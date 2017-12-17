#include <cctype>

#include "blockyvectorparser.hpp"

int BlockyVectorParser::try_parse
(
	const char* buffer,
	int offset,
	int count
)
{
	int checked = 0;

	if (!buffer[offset + checked] == '(')
	{
		return TRY_PARSE_INVALID;
	}

	for (int i = 0; i < 3; i++)
	{
		checked += scalarParser->try_parse(buffer, offset + checked, count - checked);

		if (checked == TRY_PARSE_INVALID)
			return TRY_PARSE_INVALID;

		if (checked == TRY_PARSE_BUFFER_SHORT)
			return TRY_PARSE_BUFFER_SHORT;
	}

	return checked;
}

int BlockyVectorParser::parse_constant
(
	const char* buffer,
	int offset,
	int count
)
{
	return scalarParser->parse_constant(buffer, offset, count);
}
