#pragma once
#include <venera/Parsing/Statement.h>
#include <venera/Parsing/Ast/Block.h>
#include <vector>
#include <memory>
#include <optional>
#include <venera/LanguageType.h>

namespace Venera::Parsing
{
    class FunctionDefinition final : public Statement
    {
    private:
        std::shared_ptr<Block> mBlock;
        Lexing::Token mName;
        LanguageType mReturnType;

    public:
        std::shared_ptr<Block> GetBlock() const {
            return mBlock;
        }

        const Lexing::Token& Name() const {
            return mName;
        }

        const LanguageType& ReturnType() const {
            return mReturnType;
        }

        void Dump(int indent) override;

        FunctionDefinition(std::shared_ptr<Block> block, LanguageType returnType, Lexing::Token name, Lexing::Token token) :
            Statement(std::move(token)),
            mName(std::move(name)),
            mReturnType(std::move(returnType)),
            mBlock(std::move(block)) {
        }
    };
}