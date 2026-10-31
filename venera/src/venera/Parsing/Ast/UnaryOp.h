#pragma once
#include <venera/Parsing/Expression.h>
#include <memory>

namespace Venera::Parsing
{
    class UnaryOp final : public Expression
    {
    private:
        std::shared_ptr<Expression> mExpr;

    public:
        std::shared_ptr<Expression> GetExpression() const {
            return mExpr;
        }

        void Dump(int indent) override;

        LanguageType ResolveType() override;

        UnaryOp(Lexing::Token token, std::shared_ptr<Expression> expr) :
            Expression(std::move(token)),
            mExpr(std::move(expr)) {
        }
    };
}