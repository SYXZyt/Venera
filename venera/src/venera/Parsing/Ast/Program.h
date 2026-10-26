#pragma once
#include <venera/Parsing/Statement.h>
#include <vector>
#include <memory>
#include <optional>

namespace Venera::Parsing
{
    class Program final : public Statement
    {
    private:
        std::vector<std::shared_ptr<Statement>> mStatements;
        std::optional<Lexing::Token> mProgramName;

    public:
        const std::vector<std::shared_ptr<Statement>>& Statements() const {
            return mStatements;
        }

        const std::optional<Lexing::Token>& ProgramName() const {
            return mProgramName;
        }

        void Dump(int indent) override;

        Program(std::vector<std::shared_ptr<Statement>> statements, std::optional<Lexing::Token> programName, Lexing::Token token) :
            Statement(std::move(token)),
            mProgramName(std::move(programName)),
            mStatements(std::move(statements)) {
        }
    };
}