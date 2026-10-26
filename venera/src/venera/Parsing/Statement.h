#pragma once
#include <venera/Lexing/Token.h>

namespace Venera::Parsing
{
    class Statement
    {
    protected:
        Lexing::Token mToken;

    public:
        const Lexing::Token Token() const {
            return mToken;
        }

        virtual void Dump(int indent) = 0;

        Statement(Lexing::Token token) :
            mToken(std::move(token)) {
        }

        virtual ~Statement() = default;
    };
}