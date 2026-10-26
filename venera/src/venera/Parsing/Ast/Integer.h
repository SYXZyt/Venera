#pragma once
#include <venera/Parsing/Expression.h>

namespace Venera::Parsing
{
    class Integer final : public Expression
    {
    private:
        int64 mValue;
        LanguageType mType;

    public:
        int64 Value() const {
            return mValue;
        }

        void Dump(int indent) override;

        LanguageType ResolveType() override {
            return mType;
        }

        Integer(Lexing::Token token, int64 value, LanguageType type) :
            Expression(std::move(token)),
            mValue(value),
            mType(std::move(type)) {
        }
    };
}