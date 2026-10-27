#pragma once
#include <venera/Parsing/Expression.h>

namespace Venera::Parsing
{
    class Floating final : public Expression
    {
    private:
        double mValue;
        LanguageType mType;

    public:
        int64 Value() const {
            return mValue;
        }

        void Dump(int indent) override;

        LanguageType ResolveType() override {
            return mType;
        }

        void SetType(LanguageType type) {
            mType = std::move(type);
        }

        Floating(Lexing::Token token, double value, LanguageType type) :
            Expression(std::move(token)),
            mValue(value),
            mType(std::move(type)) {
        }
    };
}