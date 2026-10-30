#include "Tokeniser.h"
#include <venera/FileTracker.h>
#include <venera/Error.h>
#include <venera/Helpers.h>
#include <sstream>

static constexpr bool IsWhitespace(char c) {
    return Venera::IsOneOf(c, ' ', '\t', '\n');
}

static constexpr bool IsDigit(char c) {
    return c >= '0' && c <= '9';
}

static constexpr bool IsIdentifier(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || IsDigit(c) || c == '_');
}

char Venera::Lexing::Tokeniser::Current() const {
    return mPos >= mContentSize ? '\0' : mContentBuffer[mPos];
}

char Venera::Lexing::Tokeniser::Peek(size_t offset) const {
    return mPos + offset >= mContentSize ? '\0' : mContentBuffer[mPos + offset];
}

void Venera::Lexing::Tokeniser::Advance(int count)
{
    mPos += count;
    mCol += count;

    if (Current() == '\n')
    {
        mCol = -1;
        ++mLine;
    }
}

void Venera::Lexing::Tokeniser::TryConsumeIntegerSuffix(std::stringstream& lexeme)
{
    if ((Current() == 'u' && Peek() == 'l') || (Current() == 'l' && Peek() == 'u'))
    {
        if (IsIdentifier(Peek(2)))
            return;

        lexeme << Current();
        lexeme << Peek();
        Advance(2);
        return;
    }

    if (Current() == 'u' || Current() == 'l' && !IsIdentifier(Peek()))
    {
        lexeme << Current();
        Advance();
    }
}

void Venera::Lexing::Tokeniser::TryConsumeFloatingSuffix(std::stringstream& lexeme)
{
    if (Current() == 'f' && !IsIdentifier(Peek()))
    {
        lexeme << Current();
        Advance();
    }
}

Venera::Lexing::Token Venera::Lexing::Tokeniser::BuildIdentifier()
{
    std::stringstream lexeme;
    int column = mCol;
    int line = mLine;

    do
    {
        lexeme << Current();
        Advance();
    } while (IsIdentifier(Current()));

    std::string lexemeStr(std::move(lexeme.str()));

    TokenType type = TokenType::IDENTIFIER;
    auto it = sKeywords.find(lexemeStr);
    if (it != sKeywords.end())
        type = it->second;

    return Token
    {
        .lexeme = std::move(lexemeStr),
        .file = mFile,
        .line = line,
        .col = column,
        .type = type,
    };
}

Venera::Lexing::Token Venera::Lexing::Tokeniser::BuildNumeric(NumericCharCheck check, TokenType type)
{
    std::stringstream lexeme;
    int column = mCol;
    int line = mLine;

    do
    {
        lexeme << Current();
        Advance();
    } while (check(Current())); // We use a delegate to check since dec and hex have different characters

    // If we are parsing a decimal (not hex) and we have a . followed by either f or more numbers
    // then we have to parse a floating point
    if (
        type == TokenType::LIT_BASE10 &&
        Current() == '.' &&
        (IsDigit(Peek()) ||
            (Peek() == 'f' && !IsIdentifier(Peek(2)))))
    {
        type = TokenType::LIT_FLOATING;

        // Consume .
        lexeme << Current();
        Advance();

        while (IsDigit(Current()))
        {
            lexeme << Current();
            Advance();
        }

        // Consume the f
        TryConsumeFloatingSuffix(lexeme);
    }
    else
    {
        // Consume u, l, lu, or ul
        TryConsumeIntegerSuffix(lexeme);
    }

    return Token
    {
        .lexeme = std::move(lexeme.str()),
        .file = mFile,
        .line = line,
        .col = column,
        .type = type,
    };
}

Venera::Lexing::Token Venera::Lexing::Tokeniser::BuildSingleCharToken(TokenType type)
{
    Token token
    {
        .lexeme = CharToString(Current()),
        .file = mFile,
        .line = mLine,
        .col = mCol,
        .type = type,
    };

    Advance();
    return token;
}

void Venera::Lexing::Tokeniser::Tokenise()
{
    while (Current() != '\0')
    {
        if (IsWhitespace(Current()))
        {
            Advance();
            continue;
        }

        // Since identifiers can contain numbers, we have to handle numbers first
        // otherwise numbers will be identifiers
        if (IsDigit(Current()))
        {
            mTokens.push_back(std::move(BuildNumeric(IsDigit, TokenType::LIT_BASE10)));
        }
        else if (IsIdentifier(Current()))
        {
            mTokens.push_back(std::move(BuildIdentifier()));
        }
        else if (Current() == '(')
        {
            mTokens.push_back(std::move(BuildSingleCharToken(TokenType::L_PAREN)));
        }
        else if (Current() == ')')
        {
            mTokens.push_back(std::move(BuildSingleCharToken(TokenType::R_PAREN)));
        }
        else if (Current() == '+')
        {
            mTokens.push_back(std::move(BuildSingleCharToken(TokenType::OP_ADD)));
        }
        else if (Current() == '-')
        {
            mTokens.push_back(std::move(BuildSingleCharToken(TokenType::OP_SUB)));
        }
        else if (Current() == '*')
        {
            mTokens.push_back(std::move(BuildSingleCharToken(TokenType::OP_MUL)));
        }
        else if (Current() == '/')
        {
            mTokens.push_back(std::move(BuildSingleCharToken(TokenType::OP_DIV)));
        }
        else
        {
            char illegalChar = Current();
            Token errToken = BuildSingleCharToken(TokenType::NONE);

            Error::Report(std::format("Illegal character '{}'", illegalChar), errToken, ErrorCode::E1000_ILLEGAL_CHARACTER);
            FileTracker::AddError(mFile);
        }
    }
}

Venera::Lexing::Tokeniser::Tokeniser(std::string_view source, std::string_view filename)
{
    mFile = File::FromSource(filename, source);
    mContentBuffer = mFile->Contents().data();
    mContentSize = mFile->Contents().size();

    // Load up the first character
    Advance();

    FileTracker::AddRegister(mFile);

    if (mContentSize == 0)
    {
        Error::Report("Source file cannot be empty", std::nullopt, ErrorCode::E0002_EMPTY_FILE);
        FileTracker::AddError(mFile);
    }
}