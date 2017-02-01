#ifndef TOCKENTYPE
#define TOCKENTYPE

enum struct TokenType
{
    // Represents the end of the stream/file.
    END_OF_STREAM = -2,

    // Represents the missingness of a token.
    NONE = -1,

    // Represents an opening brace {.
    BRACES_OPEN = 0,

    // Represents a closing brace }.
    BRACES_CLOSE = 1,

    // Represents an opening bracket [.
    BRACKETS_OPEN = 2,

    // Represents a closing bracket ].
    BRACKETS_CLOSE = 3,

    // Represents an opening parenthesis (.
    PARENTHESE_OPEN = 4,

    // Represents a closing parenthesis ).
    PARENTHESE_CLOSE = 5,

    // Represents an opening chevron &lt;.
    CHEVRONS_OPEN = 6,

    // Represents a closing chevron &gt;.
    CHEVRONS_CLOSE = 7,

    // Represents a semicolon ;.
    SEMICOLON = 8,

    // Represents a hashtag #.
    HASHTAG = 9,

    // Represnts a keyword.
    KEYWORD = 10,

    // Represents a number
    NUMBER = 11,

    // Represents a string.
    STRING = 12
};

#endif /* end of include guard: BLOCKYTOCKENTYPE */
