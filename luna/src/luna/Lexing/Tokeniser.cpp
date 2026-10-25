#include "Tokeniser.h"
#include <luna/FileTracker.h>
#include <luna/Error.h>
#include <luna/Helpers.h>
#include <sstream>

static constexpr bool IsWhitespace(char c) {
    return Luna::IsOneOf(c, ' ', '\t', '\n');
}

static constexpr bool IsDigit(char c) {
    return c >= '0' && c <= '9';
}

static constexpr bool IsIdentifier(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || IsDigit(c) || c == '_');
}

char Luna::Lexing::Tokeniser::Current() const {
    return mPos >= mContentSize ? '\0' : mContentBuffer[mPos];
}

char Luna::Lexing::Tokeniser::Peek(size_t offset) const {
    return mPos + offset >= mContentSize ? '\0' : mContentBuffer[mPos + offset];
}

void Luna::Lexing::Tokeniser::Advance(int count)
{
    mPos += count;
    mCol += count;

    if (Current() == '\n')
    {
        mCol = -1;
        ++mLine;
    }
}

Luna::Lexing::Token Luna::Lexing::Tokeniser::BuildIdentifier()
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

Luna::Lexing::Token Luna::Lexing::Tokeniser::BuildSingleCharToken(TokenType type)
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

void Luna::Lexing::Tokeniser::Tokenise()
{
    while (Current() != '\0')
    {
        if (IsWhitespace(Current()))
        {
            Advance();
            continue;
        }

        if (IsIdentifier(Current()))
        {
            mTokens.push_back(BuildIdentifier());
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

Luna::Lexing::Tokeniser::Tokeniser(std::string_view source, std::string_view filename)
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