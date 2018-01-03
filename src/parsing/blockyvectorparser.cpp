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

	if (buffer[offset + checked] != '(')
	{
		return TRY_PARSE_INVALID;
	}

	checked++;
	if (checked >= count)
		return TRY_PARSE_BUFFER_SHORT;

	for (int i = 0; i < 3; i++)
	{
		int result = scalarParser->try_parse(buffer, offset + checked, count - checked);

		if (result == TRY_PARSE_INVALID)
			return TRY_PARSE_INVALID;

		if (result == TRY_PARSE_BUFFER_SHORT)
			return TRY_PARSE_BUFFER_SHORT;

		checked += result;

		while (isspace(buffer[offset + checked]))
		{
			checked++;
			if (checked >= count)
				return TRY_PARSE_BUFFER_SHORT;
		}
	}

	if (buffer[offset + checked] != ')')
	{
		return TRY_PARSE_INVALID;
	}

	checked++;
	if (checked >= count)
		return TRY_PARSE_BUFFER_SHORT;

	return checked;
}

int BlockyVectorParser::parse_constant
(
	const char* buffer,
	int offset,
	int count
)
{
	int parsed = 0;

	parsed++;

	for (int i = 0; i < 3; i++)
	{
		int result = scalarParser->parse_constant(buffer, offset + parsed, count - parsed);

		if (result < 0)
			return result;

		parsed += result;

		while (isspace(buffer[offset + parsed]))
			parsed++;
	}

	parsed++;

	return parsed;
}
