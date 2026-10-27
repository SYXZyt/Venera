#include "IRGenerator.h"
#include <venera/Error.h>
#include <venera/FileTracker.h>
#include <llvm/IR/Verifier.h>

llvm::Type* Venera::CodeGeneration::IRGenerator::MapType(const LanguageType& type) const
{
    switch (type.kind)
    {
        case LanguageTypeKind::PRIMITIVE:
        {
            switch (type.primitiveType)
            {
                case Lexing::TokenType::TY_U8: return mBuilder->getInt8Ty();
                case Lexing::TokenType::TY_U16: return mBuilder->getInt16Ty();
                case Lexing::TokenType::TY_U32: return mBuilder->getInt32Ty();
                case Lexing::TokenType::TY_U64: return mBuilder->getInt64Ty();

                case Lexing::TokenType::TY_I8: return mBuilder->getInt8Ty();
                case Lexing::TokenType::TY_I16: return mBuilder->getInt16Ty();
                case Lexing::TokenType::TY_I32: return mBuilder->getInt32Ty();
                case Lexing::TokenType::TY_I64: return mBuilder->getInt64Ty();
            }

            break;
        }
    }

    return nullptr;
}

llvm::Constant* Venera::CodeGeneration::IRGenerator::GenerateInteger(const std::shared_ptr<Parsing::Integer>& integer)
{
    llvm::Type* type = MapType(integer->ResolveType());
    return llvm::ConstantInt::get(type, integer->Value(), !integer->ResolveType().IsUnsigned());
}

llvm::Value* Venera::CodeGeneration::IRGenerator::GenerateExpression(const std::shared_ptr<Parsing::Expression>& expr)
{
    if (std::shared_ptr integer = std::dynamic_pointer_cast<Parsing::Integer>(expr))
    {
        return GenerateInteger(integer);
    }
    else
    {
        Error::Report("No generator for expression", expr->Token(), ErrorCode::E0003_INTERNAL_COMPILER_ERROR);
        FileTracker::AddError(expr->Token().file);
        return nullptr;
    }
}

void Venera::CodeGeneration::IRGenerator::GenerateStatement(const std::shared_ptr<Parsing::Statement>& stmt)
{
    if (std::shared_ptr ret = std::dynamic_pointer_cast<Parsing::Return>(stmt))
    {
        std::shared_ptr<Parsing::Expression> returnExpr = ret->ReturnValue();

        if (returnExpr)
        {
            llvm::Value* value = GenerateExpression(ret->ReturnValue());
            mBuilder->CreateRet(value);
        }
        else
        {
            mBuilder->CreateRetVoid();
        }
    }
    else if (std::shared_ptr block = std::dynamic_pointer_cast<Parsing::Block>(stmt))
    {
        GenerateBlock(block);
    }
    else
    {
        Error::Report("No generator for statement", stmt->Token(), ErrorCode::E0003_INTERNAL_COMPILER_ERROR);
        FileTracker::AddError(stmt->Token().file);
    }
}

void Venera::CodeGeneration::IRGenerator::GenerateBlock(const std::shared_ptr<Parsing::Block>& block)
{
    for (const std::shared_ptr<Parsing::Statement>& stmt : block->Statements())
        GenerateStatement(stmt);
}

llvm::Function* Venera::CodeGeneration::IRGenerator::GenerateFunction(const std::shared_ptr<Parsing::FunctionDefinition>& func)
{
    std::vector<llvm::Type*> paramTypes;

    llvm::FunctionType* fnType = llvm::FunctionType::get(MapType(func->ReturnType()), paramTypes, false);
    llvm::Function* fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, func->Name().lexeme, mModule.get());

    mCurrentFunction = fn;

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(*mContext, "entry", fn);
    mBuilder->SetInsertPoint(entry);

    GenerateBlock(func->GetBlock());

    if (!mBuilder->GetInsertBlock()->getTerminator())
    {
        // TODO: Add mBuilder->CreateRetVoid(); once VOID is implemented    
    }

    if (llvm::verifyFunction(*fn, &llvm::errs()))
    {
        fn->eraseFromParent();
        Error::Report(std::format("Invalid IR generator for '{}'", func->Name().lexeme), func->Name(), ErrorCode::E0003_INTERNAL_COMPILER_ERROR);
        return nullptr;
    }

    return fn;
}

void Venera::CodeGeneration::IRGenerator::Generate()
{
    for (const std::shared_ptr<Parsing::Statement>& stmt : mAst->Statements())
    {
        if (std::shared_ptr funcDef = std::dynamic_pointer_cast<Parsing::FunctionDefinition>(stmt))
        {
            GenerateFunction(funcDef);
        }
        else
        {
            Error::Report("Unknown top level statement", stmt->Token(), ErrorCode::E0003_INTERNAL_COMPILER_ERROR);
            FileTracker::AddError(stmt->Token().file);
        }
    }
}

Venera::CodeGeneration::IRGenerator::IRGenerator(std::shared_ptr<Parsing::Program> ast) :
    mAst(std::move(ast)),
    mContext(std::make_unique<llvm::LLVMContext>()),
    mModule(std::make_unique<llvm::Module>("<venera>", *mContext)),
    mBuilder(std::make_unique<llvm::IRBuilder<>>(*mContext)) {
}