#pragma once
#include <vector>
#include <venera/Lexing/Token.h>
#include <venera/Lexing/TokenType.h>
#include <venera/File.h>
#include <unordered_map>
#include <functional>

namespace Venera::Lexing
{
    class Tokeniser final
    {
    private:
        using NumericCharCheck = std::function<bool(char)>;

        static inline std::unordered_map<std::string, TokenType> sKeywords =
        {
            { "PROGRAM", TokenType::KW_PROGRAM },
            { "DO", TokenType::KW_DO },
            { "END", TokenType::KW_END },
            { "NAME", TokenType::KW_END },
            { "RETURN", TokenType::KW_RETURN },

            { "U8", TokenType::TY_U8 },
            { "U16", TokenType::TY_U16 },
            { "U32", TokenType::TY_U32 },
            { "U64", TokenType::TY_U64 },

            { "I8", TokenType::TY_I8 },
            { "I16", TokenType::TY_I16 },
            { "I32", TokenType::TY_I32 },
            { "I64", TokenType::TY_I64 },
        };

        std::vector<Token> mTokens;
        std::shared_ptr<File> mFile;

        const char* mContentBuffer;
        size_t mContentSize;

        int mPos = -1;   // Position into text buffer
        int mLine = 0;  // Current line
        int mCol = -1; // Current position on the line

        char Current() const;
        char Peek(size_t offset = 1) const;

        void Advance(int count = 1);

        void TryConsumeIntegerSuffix(std::stringstream& lexeme);
        void TryConsumeFloatingSuffix(std::stringstream& lexeme);

        Token BuildIdentifier();
        Token BuildNumeric(NumericCharCheck check, TokenType type);

        Token BuildSingleCharToken(TokenType type);

    public:
        const std::vector<Token>& GetTokens() const {
            return mTokens;
        }

        void Tokenise();

        Tokeniser(std::string_view source, std::string_view filename);
    };
}