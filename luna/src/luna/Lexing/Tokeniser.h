#pragma once
#include <vector>
#include <luna/Lexing/Token.h>
#include <luna/Lexing/TokenType.h>
#include <luna/File.h>

namespace Luna::Lexing
{
    class Tokeniser final
    {
    private:
        std::vector<Token> mTokens;
        std::shared_ptr<File> mFile;

        char* mContentBuffer;
        size_t mContentSize;

        int mPos = -1;   // Position into text buffer
        int mLine = 0;  // Current line
        int mCol = -1; // Current position on the line

        bool mFailed = false;

        char Current() const;
        char Peek(size_t offset = 1) const;

        void Advance();

    public:
        bool Failed() const {
            return mFailed;
        }

        void Tokenise();

        Tokeniser(std::shared_ptr<File> file);
    };
}