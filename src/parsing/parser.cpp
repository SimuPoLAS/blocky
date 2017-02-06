#include <stdexcept>
#include <iostream>

#include <parsing/parser.hpp>

using namespace std;

Parser::Parser
(
    FILE* file,
    shared_ptr<Token> (&buffer)[bufferSize]
)
    : buffer(buffer)
    , eos(file == NULL)
    , length(0)
    , position(0)
    , size(bufferSize)
{
    hooker = make_unique<Hooker>(file, filePos);
}

// base parser methods
// TODO: decide whether to use int32_t or size_t for amount
void Parser::ensure(int32_t amount)
{
    if (amount <= length) return;
    if (eos)
        // TODO: throw meaningful exception
        throw 0;
    fill();
    if (amount <= length)
        return;
    // TODO: throw meaningful exception
    throw 0;
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

bool Parser::needs(int32_t amount)
{
    if (amount <= length)
        return true;
    if (eos)
        return false;
    fill();
    // return amount <= _length;
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
    length -= amount;
}

// void Parser::relocate();

// parser methods
bool Parser::expect(TokenType type)
{
    ensure(1);
    return buffer[position]->Type == type;
}

bool Parser::expect(TokenType types[], size_t count)
{
    if (count > 0xFFFF)
        // TODO: throw more meaningful exception
        throw 0;//new ArgumentOutOfRangeException(nameof(types));
    ensure(count);
    for (size_t i = 0; i < count; i++)
        if (buffer[position + i]->Type != types[i])
            return false;
    return true;
}

bool Parser::expect_safe(TokenType types[], size_t count)
{
    if (count > 0xFFFF)
        // TODO: throw meaningful exception
        throw 0;//new ArgumentOutOfRangeException(nameof(types));
    if (!needs(count))
        return false;
    for (size_t i = 0; i < count; i++)
        if (buffer[position + i]->Type != types[i])
            return false;
    return true;
}

void Parser::parse(int w)
{
    length = w;
    position = 0;
    try
    {
        while (needs(1))
        {
            auto tkn = buffer[position];

            if (hooker->is_in_list())
            {
                std::cout << "continuing list" << '\n';
                parse_list_continue(hooker->get_type());
                continue;
            }

            if
            (
                tkn->Type == TokenType::KEYWORD
             || tkn->Type == TokenType::STRING
            )
            {
                skip(1);
                parse_entry_or_object(*tkn);
                continue;
            }

            if
            (
                tkn->Type == TokenType::NUMBER
             && needs(2)
             && buffer[position + 1]->Type == TokenType::PARENTHESE_OPEN
            )
            {
                skip(2);
                try
                {
                    double a = stod(tkn->Payload);
                    parse_anonymous_list(int32_t(a));
                }
                catch (invalid_argument const& ia)
                {
                    parse_anonymous_list();
                }
                continue;
            }

            if (tkn->Type == TokenType::PARENTHESE_OPEN)
            {
                skip(1);
                parse_anonymous_list();
                continue;
            }

            if (tkn->Type == TokenType::HASHTAG)
            {
                parse_directive();
                continue;
            }

            if (tkn->Type == TokenType::SEMICOLON)
            {
                skip(1);
                continue;
            }

            // TODO: throw meaningful exception
            throw 0;//new ParserException();
        }
    }
    catch (int e)
    {
        if (e == 404)
            std::cout << "buffer has to be reload" << '\n';
        else
            std::cout << "some error occured" << '\n';
    }
}

void Parser::parse_entry_or_object(Token const& me)
{
    if (!needs(1))
        // TODO: throw meaningful exception
        throw 0;//new ParserException();
    auto c = *buffer[position];

    if (c.Type == TokenType::BRACES_OPEN)
    {
        skip(1);
        parse_object(me);
    }
    else if (c.Type == TokenType::HASHTAG)
    {
        skip(1);
        parse_code_stream_object(me);
    }
    else
    {
        parse_entry(me);
    }
}

void Parser::parse_object(Token const& me)
{
    hooker->enter_dictionary(me.Payload);
    while (!(eos && length == 0))
    {
        if (!needs(1))
            // TODO: throw meaningful exception
            throw 0;//new ParserException();
        auto tkn = buffer[position];
        if (tkn->Type == TokenType::KEYWORD || tkn->Type == TokenType::STRING)
        {
            skip(1);
            parse_entry_or_object(*tkn);
        }
        else if
        (
            tkn->Type == TokenType::NUMBER
         || tkn->Type == TokenType::PARENTHESE_OPEN
        )
            parse_anonymous_list();
        else if (tkn->Type == TokenType::SEMICOLON)
            skip(1);
        else if (tkn->Type == TokenType::BRACES_CLOSE)
        {
            hooker->leave_dictionary();
            skip(1);
            return;
        }
        else if (tkn->Type == TokenType::HASHTAG)
            parse_directive();
        else
            // TODO: throw meaningful exception
            throw 0;//new ParserException();
    }
    // TODO: throw meaningful exception
    throw 0;//new ParserException();
}

void Parser::parse_code_stream_object(Token const& me)
{
    if (!expect(TokenType::KEYWORD))
        // TODO: throw meaningful exception
        throw 0;//new ParserException();
    auto text = buffer[position]->Payload;
    if (text != "codeStream")
        // TODO: throw meaningful exception
        throw 0;//new ParserException();
    skip(1);
    if (!expect(TokenType::BRACES_OPEN))
        // TODO: throw meaningful exception
        throw 0;//new ParserException();
    skip(1);
    hooker->enter_code_stream_dictionary(me.Payload);
    while (!(eos && length == 0))
    {
        if (!needs(1))
            // TODO: throw meaningful exception
            throw 0;//new ParserException();
        auto tkn = buffer[position];
        if (tkn->Type == TokenType::KEYWORD || tkn->Type == TokenType::STRING)
        {
            skip(1);
            parse_entry_or_object(*tkn);
        }
        else if
        (
            tkn->Type == TokenType::NUMBER
         || tkn->Type == TokenType::PARENTHESE_OPEN
        )
            parse_anonymous_list();
        else if (tkn->Type == TokenType::SEMICOLON)
            skip(1);
        else if (tkn->Type == TokenType::BRACES_CLOSE)
        {
            hooker->leave_code_stream_dictionary();
            skip(1);
            return;
        }
        else if (tkn->Type == TokenType::HASHTAG)
            parse_directive();
            // TODO: throw meaningful exception
            throw 0;//new ParserException();
    }
    // TODO: throw meaningful exception
    throw 0;//new ParserException();
}

void Parser::parse_entry(Token const& me)
{
    hooker->enter_entry(me.Payload);
    while (!(eos && length == 0))
    {
        if (!needs(1))
            // TODO: throw meaningful exception
            throw 0;//new ParserException();
        auto tkn = buffer[position];
        if (tkn->Type == TokenType::SEMICOLON)
        {
            hooker->leave_entry();
            skip(1);
            return;
        }
        parse_value(*tkn);
    }
    // TODO: throw meaningful exception
    throw 0;//new ParserException();
}

void Parser::parse_directive()
{
    if (buffer[position]->Type != TokenType::HASHTAG)
        // TODO: throw meaningful exception
        throw 0;//new ParserException();
    skip();
    expect(TokenType::STRING);
    auto macro = buffer[position]->Payload;
    DirectiveType type;

    if (macro == "include")
        type = DirectiveType::Include;
    else if (macro == "inputMode")
        type = DirectiveType::InputMode;
    else if (macro == "remove")
        type = DirectiveType::Remove;
    else
        // TODO: throw meaningful exception
        throw 0;//new ParserException();

    skip();
    ensure(1);
    if
    (
        buffer[position]->Type != TokenType::KEYWORD
     && buffer[position]->Type != TokenType::STRING
    )
        // TODO: throw meaningful exception
        throw 0;//new ParserException();
    hooker->handle_macro(type, buffer[position]->Payload);
    skip();
}

void Parser::parse_value(Token const& me)
{
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
                if(!expect(types, 3))
                    // TODO: throw meaningful exception
                    throw 0;//new ParserException();
                auto ltype = buffer[position + 1]->Payload;
                ListType type;

                if (ltype == "scalar")
                    type = ListType::Scalar;
                else if (ltype == "vector")
                    type = ListType::Vector;
                else if (ltype == "tensor")
                    type = ListType::Tensor;
                else
                    // TODO: throw meaningful exception
                    throw 0;//new ParserException();

                skip(3);
                if (!needs(1))
                    // TODO: throw meaningful exception
                    throw 0;//new ParserException();
                auto amount = -1;
                auto c = *buffer[position];
                if (c.Type == TokenType::NUMBER)
                {
                    amount = (int)stod(c.Payload);
                    skip(1);
                }
                if (!expect(TokenType::PARENTHESE_OPEN))
                    // TODO: throw meaningful exception
                    throw 0;//new ParserException();
                skip(1);
                parse_list(type, amount);
                return;
            }

            skip(1);
            hooker->handle_keyword(me.Payload);
            return;
        case TokenType::NUMBER:
            if
            (
                needs(2)
             && buffer[position + 1]->Type == TokenType::PARENTHESE_OPEN
            )
            {
                skip(2);
                try
                {
                    double a = stod(me.Payload);
                    parse_anonymous_list(int32_t(a));
                }
                catch (invalid_argument const& ia)
                {
                    parse_anonymous_list();
                }
                return;
            }

            skip(1);
            hooker->handle_scalar(me.Payload);
            return;
        case TokenType::STRING:
            skip(1);
            hooker->handle_string(me.Payload);
            return;
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
            if (!expect(types, 8))
                // TODO: throw meaningful exception
                throw 0;//new ParserException();
            string values[7];
            for (size_t i = 0; i < 7; i++)
                values[i] = buffer[position + i]->Payload;
            skip(8);
            hooker->handle_dimension(values, 7);
            return;
        }
        case TokenType::PARENTHESE_OPEN:
            skip(1);
            parse_anonymous_list();
            return;
        default:
            // TODO: throw meaningful exception
            throw 0;//new ParserException();
    }
}

void Parser::parse_list(ListType type, int32_t amount)
{
    hooker->enter_list(type, amount);
    parse_list_continue(type);
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

void Parser::parse_list_continue(ListType type)
{
    Token c;
    auto done = false;
    auto d = false;
    do
    {
        // HERE
        if (!needs(1))
            // TODO: throw meaningful exception
            throw 0;//new ParserException();
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
                // TODO: throw meaningful exception
                throw 0;//new Exception("Not supported list type: " + type);
        }
    } while (!(eos && length == 0));
    if (!d)
        hooker->leave_list();
    if (!done)
        // TODO: throw meaningful exception
        throw 0;//new ParserException();
}

void Parser::parse_anonymous_list(int32_t number)
{
    if (!needs(1))
        // TODO: throw meaningful exception
        throw 0;//new ParserException();
    auto c = *buffer[position];
    if (SimpleAnonyomousLists)
    {
        if (c.Type == TokenType::NUMBER)
        {
            parse_list(ListType::Scalar, number);
            return;
        }

        TokenType t[] =
        {
            TokenType::PARENTHESE_OPEN,
            TokenType::NUMBER,
            TokenType::NUMBER,
            TokenType::NUMBER,
            TokenType::PARENTHESE_CLOSE,
        };
        if (expect_safe(t, 5))
        {
            parse_list(ListType::Vector, number);
            return;
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
        if (expect_safe(t2, 11))
        {
            parse_list(ListType::Tensor, number);
            return;
        }
    }
    hooker->enter_list(ListType::Anonymous, number);
    for (; c.Type != TokenType::PARENTHESE_CLOSE; c = *buffer[position])
    {
        if
        (
            c.Type == TokenType::KEYWORD
         && needs(2)
         && buffer[position + 1]->Type == TokenType::BRACES_OPEN
        )
        {
            skip(2);
            parse_object(c);
        }
        else
            parse_value(c);
        if (!needs(1))
            // TODO: throw meaningful exception
            throw 0;//new Exception("Not supported list type: " + type);
    }
    hooker->leave_list();
    skip(1);
}

void Parser::parse_scalar()
{
    if (!expect(TokenType::NUMBER))
        // TODO: throw meaningful exception
        throw 0;//new Exception("Not supported list type: " + type);
    hooker->handle_list_entry(buffer[position]->Payload);
    skip(1);
}

void Parser::parse_vector()
{
    TokenType t[] =
    {
        TokenType::PARENTHESE_OPEN,
        TokenType::NUMBER,
        TokenType::NUMBER,
        TokenType::NUMBER,
        TokenType::PARENTHESE_CLOSE
    };
    if (!expect(t, 5))
        // TODO: throw meaningful exception
        throw 0;//new Exception("Not supported list type: " + type);
    string values[3];
    for (auto i = 0; i < 3; i++)
        values[i] = buffer[position + i + 1]->Payload;
    hooker->handle_list_entries(values, 3);
    skip(5);
}

void Parser::parse_tensor()
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
    if (!expect(t, 11))
        // TODO: throw meaningful exception
        throw 0;//new Exception("Not supported list type: " + type);
    string values[9];
    for (size_t i = 0; i < 9; i++)
        values[i] = buffer[position + i + 1]->Payload;
    hooker->handle_list_entries(values, 9);
    skip(11);
}


uint32_t Parser::get_position() { return buffer[position]->Position; }

// public
