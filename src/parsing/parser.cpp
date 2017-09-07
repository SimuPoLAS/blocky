#include <stdexcept>
#include <iostream>

#include <parsing/parser.hpp>

using namespace std;

Parser::Parser
(
	FILE* file,
	shared_ptr<Token>(&buffer)[bufferSize]
)
	: buffer(buffer)
	, eos(file == NULL)
	, length(0)
	, position(0)
	, size(bufferSize)
	, filePos(0)
	, hooker(nullptr)
{
	hooker = make_unique<Hooker>(file, filePos);
}

// base parser methods
// TODO: decide whether to use int32_t or size_t for amount
ParserResult Parser::ensure(int32_t amount)
{
	if (amount <= length) return ParserResult::SUCCESS;
	if (eos) return ParserResult::UNEXPECTED_EOF;
	return ParserResult::END_OF_BUFFER;
}

void Parser::fill()
{
	// NOTE: If fill is requested, we just quit with 404
	// at this point it can not process further
	// it has to wait for another flush_buffer call in streambuf
	// TODO: maybe find better solution
	// TODO: throw meaningful exception
	throw 404;
}

ParserResult Parser::needs(int32_t amount)
{
	if (amount <= length) return ParserResult::SUCCESS;
	if (eos) return ParserResult::UNEXPECTED_EOF;
	return ParserResult::END_OF_BUFFER;
}

void Parser::skip()
{
	position++;
	filePos++;
	length--;
}

void Parser::skip(int32_t amount)
{
	position += amount;
	filePos += amount;
	length -= amount;
}

// void Parser::relocate();

// parser methods
ParserResult Parser::expect(TokenType type)
{
	auto result = needs(1);
	if (result != ParserResult::SUCCESS)
		return result;
	return buffer[position]->Type == type
		? ParserResult::SUCCESS
		: ParserResult::UNEXPECTED_TOKEN;
}

ParserResult Parser::expect(TokenType types[], size_t count)
{
	auto result = needs(count);
	if (result != ParserResult::SUCCESS)
		return result;
	for (size_t i = 0; i < count; i++)
		if (buffer[position + i]->Type != types[i])
			return ParserResult::UNEXPECTED_TOKEN;
	return ParserResult::SUCCESS;
}

void Parser::parse(int w)
{
	length = w;
	position = 0;

	auto result = ParserResult::SUCCESS;
	auto needsres = ParserResult::SUCCESS;
	while ((needsres = needs(1)) == ParserResult::SUCCESS)
	{
		if (result != ParserResult::SUCCESS)
			break;

		auto tkn = buffer[position];

		if (hooker->is_in_list())
		{
			std::cout << "continuing list" << '\n';
			result = parse_list_continue(hooker->get_type());
			if (result != ParserResult::SUCCESS)
				break;
			continue;
		}

		if
		(
			tkn->Type == TokenType::KEYWORD
		 || tkn->Type == TokenType::STRING
		)
		{
			skip(1);
			result = parse_entry_or_object(*tkn);
			if (result != ParserResult::SUCCESS)
				break;
			continue;
		}

		result = needs(2);

		if
		(
			tkn->Type == TokenType::NUMBER
		 && result != ParserResult::SUCCESS
		 && buffer[position + 1]->Type == TokenType::PARENTHESE_OPEN
		)
		{
			skip(2);
			try
			{
				// TODO: check whether string can be parsed 
				// to double with an invalid argument exception
				double a = stod(tkn->Payload);
				result = parse_anonymous_list(int32_t(a));
			}
			catch (invalid_argument const& ia)
			{
				result = parse_anonymous_list();
			}
			if (result != ParserResult::SUCCESS)
				break;
			continue;
		}

		if (tkn->Type == TokenType::PARENTHESE_OPEN)
		{
			skip(1);
			result = parse_anonymous_list();
			if (result != ParserResult::SUCCESS)
				break;
			continue;
		}

		if (tkn->Type == TokenType::HASHTAG)
		{
			result = parse_directive();
			if (result != ParserResult::SUCCESS)
				break;
			continue;
		}

		if (tkn->Type == TokenType::SEMICOLON)
		{
			skip(1);
			continue;
		}

		result = ParserResult::UNEXPECTED_TOKEN;
		break;
	}

	if ((int)needsres < 0)
		exit(-1);

	if ((int)result < 0)
		exit(-1);
}

ParserResult Parser::parse_entry_or_object(Token const& me)
{
	auto result = needs(1);
	if (result != ParserResult::SUCCESS)
		return result;

	auto c = *buffer[position];

	if (c.Type == TokenType::BRACES_OPEN)
	{
		skip(1);
		result = parse_object(me);
		if (result != ParserResult::SUCCESS)
			return result;
	}
	else if (c.Type == TokenType::HASHTAG)
	{
		skip(1);
		result = parse_code_stream_object(me);
		if (result != ParserResult::SUCCESS)
			return result;
	}
	else
	{
		result = parse_entry(me);
		if (result != ParserResult::SUCCESS)
			return result;
	}

	return ParserResult::SUCCESS;
}

ParserResult Parser::parse_object(Token const& me)
{
	ParserResult result = ParserResult::SUCCESS;
	hooker->enter_dictionary(me.Payload);
	while (!(eos && length == 0))
	{
		result = needs(1);
		if (result != ParserResult::SUCCESS)
			return result;

		auto tkn = buffer[position];
		if (tkn->Type == TokenType::KEYWORD || tkn->Type == TokenType::STRING)
		{
			skip(1);
			result = parse_entry_or_object(*tkn);
			if (result != ParserResult::SUCCESS)
				return result;
		}
		else if
			(
				tkn->Type == TokenType::NUMBER
				|| tkn->Type == TokenType::PARENTHESE_OPEN
				)
		{
			result = parse_anonymous_list();
			if (result != ParserResult::SUCCESS)
				return result;
		}
		else if (tkn->Type == TokenType::SEMICOLON)
		{
			skip(1);
		}
		else if (tkn->Type == TokenType::BRACES_CLOSE)
		{
			hooker->leave_dictionary();
			skip(1);
			return ParserResult::SUCCESS;
		}
		else if (tkn->Type == TokenType::HASHTAG)
		{
			parse_directive();
			if (result != ParserResult::SUCCESS)
				return result;
		}
		else
		{
			return ParserResult::UNEXPECTED_TOKEN;
		}
	}
	return ParserResult::UNEXPECTED_EOF;
}

ParserResult Parser::parse_code_stream_object(Token const& me)
{
	auto result = expect(TokenType::KEYWORD);
	if (result != ParserResult::SUCCESS)
		return result;

	auto text = buffer[position]->Payload;
	if (text != "codeStream")
		return ParserResult::UNEXPECTED_TOKEN;

	skip(1);
	result = expect(TokenType::BRACES_OPEN);
	if (result != ParserResult::SUCCESS)
		return result;

	skip(1);
	hooker->enter_code_stream_dictionary(me.Payload);
	while (!(eos && length == 0))
	{
		result = needs(1);
		if (result != ParserResult::SUCCESS)
			return result;

		auto tkn = buffer[position];
		if (tkn->Type == TokenType::KEYWORD || tkn->Type == TokenType::STRING)
		{
			skip(1);
			result = parse_entry_or_object(*tkn);
			if (result != ParserResult::SUCCESS)
				return result;
		}
		else if
			(
				tkn->Type == TokenType::NUMBER
				|| tkn->Type == TokenType::PARENTHESE_OPEN
				)
		{
			parse_anonymous_list();
			if (result != ParserResult::SUCCESS)
				return result;
		}
		else if (tkn->Type == TokenType::SEMICOLON)
		{
			skip(1);
		}
		else if (tkn->Type == TokenType::BRACES_CLOSE)
		{
			hooker->leave_code_stream_dictionary();
			skip(1);
			return ParserResult::SUCCESS;
		}
		else if (tkn->Type == TokenType::HASHTAG)
		{
			parse_directive();
			if (result != ParserResult::SUCCESS)
				return result;
		}

		return ParserResult::UNEXPECTED_TOKEN;
	}
	return ParserResult::UNEXPECTED_EOF;
}

ParserResult Parser::parse_entry(Token const& me)
{
	ParserResult result = ParserResult::SUCCESS;
	hooker->enter_entry(me.Payload);
	while (!(eos && length == 0))
	{
		result = needs(1);
		if (result != ParserResult::SUCCESS)
			return result;

		auto tkn = buffer[position];
		if (tkn->Type == TokenType::SEMICOLON)
		{
			hooker->leave_entry();
			skip(1);
			return ParserResult::SUCCESS;
		}

		result = parse_value(*tkn);
		if (result != ParserResult::SUCCESS)
			return result;
	}
	return ParserResult::UNEXPECTED_EOF;
}

ParserResult Parser::parse_directive()
{
	if (buffer[position]->Type != TokenType::HASHTAG)
		return ParserResult::UNEXPECTED_TOKEN;

	skip();

	auto result = expect(TokenType::STRING);
	if (result != ParserResult::SUCCESS)
		return result;

	auto macro = buffer[position]->Payload;
	DirectiveType type;

	if (macro == "include")
		type = DirectiveType::Include;
	else if (macro == "inputMode")
		type = DirectiveType::InputMode;
	else if (macro == "remove")
		type = DirectiveType::Remove;
	else
		return ParserResult::UNEXPECTED_TOKEN;

	skip();
	result = ensure(1);
	if (result != ParserResult::SUCCESS)
		return result;

	if
		(
			buffer[position]->Type != TokenType::KEYWORD
			&& buffer[position]->Type != TokenType::STRING
			)
		return ParserResult::UNEXPECTED_TOKEN;

	hooker->handle_macro(type, buffer[position]->Payload);
	skip();
}

ParserResult Parser::parse_value(Token const& me)
{
	ParserResult result = ParserResult::SUCCESS;

	switch (me.Type)
	{
	case TokenType::KEYWORD:
		if (me.Payload == "List")
		{
			skip(1);
			TokenType types[] =
			{
				TokenType::CHEVRONS_OPEN,
				TokenType::KEYWORD,
				TokenType::CHEVRONS_CLOSE
			};
			result = expect(types, 3);
			if (result != ParserResult::SUCCESS)
				return result;

			auto ltype = buffer[position + 1]->Payload;
			ListType type;

			if (ltype == "scalar")
				type = ListType::Scalar;
			else if (ltype == "vector")
				type = ListType::Vector;
			else if (ltype == "tensor")
				type = ListType::Tensor;
			else
				return ParserResult::UNEXPECTED_TOKEN;

			skip(3);
			result = needs(1);
			if (result != ParserResult::SUCCESS)
				return result;

			auto amount = -1;
			auto c = *buffer[position];
			if (c.Type == TokenType::NUMBER)
			{
				amount = (int)stod(c.Payload);
				skip(1);
			}

			result = expect(TokenType::PARENTHESE_OPEN);
			if (result != ParserResult::SUCCESS)
				return result;

			skip(1);
			result = parse_list(type, amount);
			if (result != ParserResult::SUCCESS)
				return result;

			return ParserResult::SUCCESS;
		}

		skip(1);
		hooker->handle_keyword(me.Payload);
		return ParserResult::SUCCESS;
	case TokenType::NUMBER:
		result = needs(2);
		if
			(
				result != ParserResult::SUCCESS
				&& buffer[position + 1]->Type == TokenType::PARENTHESE_OPEN
				)
		{
			skip(2);
			try
			{
				// TODO: check whether string can be parsed 
				// to double with an invalid argument exception
				double a = stod(me.Payload);
				result = parse_anonymous_list(int32_t(a));
				if (result != ParserResult::SUCCESS)
					return result;
			}
			catch (invalid_argument const& ia)
			{
				result = parse_anonymous_list();
				if (result != ParserResult::SUCCESS)
					return result;
			}
			return ParserResult::SUCCESS;
		}

		skip(1);
		hooker->handle_scalar(me.Payload);
		return ParserResult::SUCCESS;
	case TokenType::STRING:
		skip(1);
		hooker->handle_string(me.Payload);
		return ParserResult::SUCCESS;
	case TokenType::BRACKETS_OPEN:
	{
		skip(1);
		TokenType types[] =
		{
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::BRACKETS_CLOSE
		};
		result = expect(types, 8);
		if (result != ParserResult::SUCCESS)
			return result;

		string values[7];
		for (size_t i = 0; i < 7; i++)
			values[i] = buffer[position + i]->Payload;
		skip(8);

		hooker->handle_dimension(values, 7);
		return ParserResult::SUCCESS;
	}
	case TokenType::PARENTHESE_OPEN:
		skip(1);
		result = parse_anonymous_list();
		if (result != ParserResult::SUCCESS)
			return result;

		return ParserResult::SUCCESS;
	default:
		return ParserResult::UNEXPECTED_TOKEN;
	}
}

ParserResult Parser::parse_list(ListType type, int32_t amount)
{
	hooker->enter_list(type, amount);
	return parse_list_continue(type);
	// Token c;
	// auto done = false;
	// auto d = false;
	// do
	// {
	//     // HERE
	//     if (!needs(1))
	//         // TODO: throw meaningful exception
	//         throw 0;//new ParserException();
	//     c = *buffer[position];
	//     if (c.Type == TokenType::PARENTHESE_CLOSE)
	//     {
	//         d = true;
	//         hooker->leave_list();
	//         skip(1);
	//         done = true;
	//         break;
	//     }
	//     switch (type)
	//     {
	//         case ListType::Scalar:
	//             parse_scalar();
	//             break;
	//         case ListType::Vector:
	//             parse_vector();
	//             break;
	//         case ListType::Tensor:
	//             parse_tensor();
	//             break;
	//         default:
	//             // TODO: throw meaningful exception
	//             throw 0;//new Exception("Not supported list type: " + type);
	//     }
	// } while (!(eos && length == 0));
	// if (!d)
	//     hooker->leave_list();
	// if (!done)
	//     // TODO: throw meaningful exception
	//     throw 0;//new ParserException();
}

ParserResult Parser::parse_list_continue(ListType type)
{
	Token c;
	auto done = false;
	auto d = false;
	do
	{
		// HERE
		auto result = needs(1);
		if (result != ParserResult::SUCCESS)
			return result;

		c = *buffer[position];
		if (c.Type == TokenType::PARENTHESE_CLOSE)
		{
			d = true;
			hooker->leave_list();
			skip(1);
			done = true;
			break;
		}
		switch (type)
		{
		case ListType::Scalar:
			parse_scalar();
			break;
		case ListType::Vector:
			parse_vector();
			break;
		case ListType::Tensor:
			parse_tensor();
			break;
		default:
			return ParserResult::UNEXPECTED_TOKEN;
		}
	} while (!(eos && length == 0));
	if (!d)
	{
		hooker->leave_list();
		return ParserResult::SUCCESS;
	}
	if (!done)
		return ParserResult::UNEXPECTED_TOKEN;
	return ParserResult::SUCCESS;
}

ParserResult Parser::parse_anonymous_list(int32_t number)
{
	auto result = needs(1);
	if (result != ParserResult::SUCCESS)
		return result;

	auto c = *buffer[position];
	if (SimpleAnonyomousLists)
	{
		if (c.Type == TokenType::NUMBER)
		{
			result = parse_list(ListType::Scalar, number);
			return result;
		}

		TokenType t[] =
		{
			TokenType::PARENTHESE_OPEN,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::PARENTHESE_CLOSE,
		};
		result = expect(t, 5);
		if (result == ParserResult::SUCCESS)
		{
			result = parse_list(ListType::Vector, number);
			return result;
		}

		TokenType t2[] =
		{
			TokenType::PARENTHESE_OPEN,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::NUMBER,
			TokenType::PARENTHESE_CLOSE,
		};
		result = expect(t2, 11);
		if (result == ParserResult::SUCCESS)
		{
			result = parse_list(ListType::Tensor, number);
			return result;
		}
	}
	hooker->enter_list(ListType::Anonymous, number);
	for (; c.Type != TokenType::PARENTHESE_CLOSE; c = *buffer[position])
	{
		result = needs(2);
		if
			(
				c.Type == TokenType::KEYWORD
				&& result == ParserResult::SUCCESS
				&& buffer[position + 1]->Type == TokenType::BRACES_OPEN
				)
		{
			skip(2);
			result = parse_object(c);
			if (result != ParserResult::SUCCESS)
				return result;
		}
		else
		{
			result = parse_value(c);
			if (result != ParserResult::SUCCESS)
				return result;
		}

		result = needs(1);
		if (result != ParserResult::SUCCESS)
			return result;
	}
	hooker->leave_list();
	skip(1);
}

ParserResult Parser::parse_scalar()
{
	auto result = expect(TokenType::NUMBER);
	if (result != ParserResult::SUCCESS)
		return result;

	hooker->handle_list_entry(buffer[position]->Payload);
	skip(1);

	return ParserResult::SUCCESS;
}

ParserResult Parser::parse_vector()
{
	TokenType t[] =
	{
		TokenType::PARENTHESE_OPEN,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::PARENTHESE_CLOSE
	};
	auto result = expect(t, 5);
	if (result != ParserResult::SUCCESS)
		return result;

	string values[3];
	for (auto i = 0; i < 3; i++)
		values[i] = buffer[position + i + 1]->Payload;
	hooker->handle_list_entries(values, 3);
	skip(5);

	return ParserResult::SUCCESS;
}

ParserResult Parser::parse_tensor()
{
	TokenType t[] =
	{
		TokenType::PARENTHESE_OPEN,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::NUMBER,
		TokenType::PARENTHESE_CLOSE
	};
	auto result = expect(t, 11);
	if (result != ParserResult::SUCCESS)
		return result;

	string values[9];
	for (size_t i = 0; i < 9; i++)
		values[i] = buffer[position + i + 1]->Payload;
	hooker->handle_list_entries(values, 9);
	skip(11);

	return ParserResult::SUCCESS;
}


uint32_t Parser::get_position() { return buffer[position]->Position; }

// public
