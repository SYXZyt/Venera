#pragma once
#include <venera/Parsing/Expression.h>

namespace Venera::Parsing
{
    class Boolean final : public Expression
    {
    private:
        bool mValue;

    public:
        bool Value() const {
            return mValue;
        }

        void Dump(int indent) override;

        LanguageType ResolveType() override {
            return LanguageType::Boolean;
        }

        Boolean(Lexing::Token token, bool value) :
            Expression(std::move(token)),
            mValue(value) {
        }
    };
}