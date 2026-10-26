#pragma once
#include <venera/Parsing/Statement.h>
#include <venera/Parsing/Expression.h>
#include <vector>
#include <memory>
#include <optional>

namespace Venera::Parsing
{
    class Return final : public Statement
    {
    private:
        std::shared_ptr<Expression> mReturnValue;

    public:
        std::shared_ptr<Expression> ReturnValue() const {
            return mReturnValue;
        }

        void Dump(int indent) override;

        Return(std::shared_ptr<Expression> returnValue, Lexing::Token token) :
            Statement(std::move(token)),
            mReturnValue(std::move(returnValue)) {
        }
    };
}