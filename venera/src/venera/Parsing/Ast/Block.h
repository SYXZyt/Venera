#pragma once
#include <venera/Parsing/Statement.h>
#include <vector>
#include <memory>
#include <optional>

namespace Venera::Parsing
{
    class Block final : public Statement
    {
    private:
        std::vector<std::shared_ptr<Statement>> mStatements;

    public:
        const std::vector<std::shared_ptr<Statement>>& Statements() const {
            return mStatements;
        }

        void Dump(int indent) override;

        Block(Lexing::Token token, std::vector<std::shared_ptr<Statement>> statements) :
            Statement(std::move(token)),
            mStatements(std::move(statements)) {
        }
    };
}