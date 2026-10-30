#pragma once
#include <venera/Parsing/Expression.h>
#include <memory>

namespace Venera::Parsing
{
    class BinaryOp final : public Expression
    {
    private:
        std::shared_ptr<Expression> mLhs;
        std::shared_ptr<Expression> mRhs;
        Lexing::Token mOp;

    public:
        const Lexing::Token& Operator() const {
            return mOp;
        }

        std::shared_ptr<Expression> Left() const {
            return mLhs;
        }

        std::shared_ptr<Expression> Right() const {
            return mRhs;
        }

        void Dump(int indent) override;

        LanguageType ResolveType() override;

        BinaryOp(Lexing::Token token, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right, Lexing::Token op) :
            Expression(std::move(token)),
            mLhs(std::move(left)),
            mRhs(std::move(right)),
            mOp(op) {
        }
    };
}