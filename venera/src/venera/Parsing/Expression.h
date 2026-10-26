#pragma once
#include <venera/Lexing/Token.h>
#include <venera/LanguageType.h>

namespace Venera::Parsing
{
    class Expression
    {
    protected:
        Lexing::Token mToken;

    public:
        const Lexing::Token Token() const {
            return mToken;
        }

        virtual void Dump(int indent) = 0;

        virtual LanguageType ResolveType() = 0;

        Expression(Lexing::Token token) :
            mToken(std::move(token)) {
        }

        virtual ~Expression() = default;
    };
}