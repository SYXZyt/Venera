#pragma once
#include <venera/Parsing/Ast.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <venera/LanguageType.h>

namespace Venera::CodeGeneration
{
    class IRGenerator final
    {
    private:
        std::shared_ptr<Parsing::Program> mAst;
        std::unique_ptr<llvm::LLVMContext> mContext;
        std::unique_ptr<llvm::Module> mModule;
        std::unique_ptr<llvm::IRBuilder<>> mBuilder;

        llvm::Function* mCurrentFunction = nullptr;

        llvm::Type* MapType(const LanguageType& type) const;

        llvm::Constant* GenerateInteger(const std::shared_ptr<Parsing::Integer>& integer);
        llvm::Constant* GenerateFloat(const std::shared_ptr<Parsing::Floating>& floating);
        llvm::Value* GenerateExpression(const std::shared_ptr<Parsing::Expression>& expr);

        void GenerateStatement(const std::shared_ptr<Parsing::Statement>& stmt);
        void GenerateBlock(const std::shared_ptr<Parsing::Block>& block);

        llvm::Function* GenerateFunction(const std::shared_ptr<Parsing::FunctionDefinition>& func);

    public:
        void PrintIR() const;

        void Generate();

        bool EmitObject(const std::string& path);

        IRGenerator(std::shared_ptr<Parsing::Program> ast);
    };
}