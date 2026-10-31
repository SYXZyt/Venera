#include "IRGenerator.h"
#include <venera/Error.h>
#include <venera/FileTracker.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>

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

                case Lexing::TokenType::TY_F32: return mBuilder->getFloatTy();
                case Lexing::TokenType::TY_F64: return mBuilder->getDoubleTy();
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

llvm::Constant* Venera::CodeGeneration::IRGenerator::GenerateFloat(const std::shared_ptr<Parsing::Floating>& floating)
{
    llvm::Type* type = MapType(floating->ResolveType());
    return llvm::ConstantFP::get(type, floating->Value());
}

llvm::Value* Venera::CodeGeneration::IRGenerator::GenerateExpression(const std::shared_ptr<Parsing::Expression>& expr)
{
    if (std::shared_ptr integer = std::dynamic_pointer_cast<Parsing::Integer>(expr))
    {
        return GenerateInteger(integer);
    }
    else if (std::shared_ptr floating = std::dynamic_pointer_cast<Parsing::Floating>(expr))
    {
        return GenerateFloat(floating);
    }
    else if (std::shared_ptr binop = std::dynamic_pointer_cast<Parsing::BinaryOp>(expr))
    {
        return GenerateBinaryOperation(binop);
    }
    else if (std::shared_ptr unary = std::dynamic_pointer_cast<Parsing::UnaryOp>(expr))
    {
        return GenerateUnaryOperation(unary);
    }
    else
    {
        Error::Report("No generator for expression", expr->Token(), ErrorCode::E0003_INTERNAL_COMPILER_ERROR);
        FileTracker::AddError(expr->Token().file);
        return nullptr;
    }
}

llvm::Value* Venera::CodeGeneration::IRGenerator::GenerateBinaryOperation(const std::shared_ptr<Parsing::BinaryOp>& binop)
{
    llvm::Value* lhs = GenerateExpression(binop->Left());
    llvm::Value* rhs = GenerateExpression(binop->Right());

    if (!lhs || !rhs)
        return nullptr;

    LanguageType exprType = binop->ResolveType();

    if (exprType == LanguageType::Runtime)
    {
        Error::Report("Cannot generate expression", binop->Token(), ErrorCode::E0003_INTERNAL_COMPILER_ERROR);
        FileTracker::AddError(binop->Token().file);
        return nullptr;
    }

    bool isSigned = !exprType.IsUnsigned();

    switch (binop->Operator().type)
    {
        case Lexing::TokenType::OP_ADD:
            return mBuilder->CreateAdd(lhs, rhs, "add");

        case Lexing::TokenType::OP_SUB:
            return mBuilder->CreateSub(lhs, rhs, "sub");

        case Lexing::TokenType::OP_MUL:
            return mBuilder->CreateMul(lhs, rhs, "mul");

        case Lexing::TokenType::OP_DIV:
            return isSigned ? mBuilder->CreateSDiv(lhs, rhs, "div") : mBuilder->CreateUDiv(lhs, rhs, "div");

        default:
            Error::Report("Unknown operator", binop->Token(), ErrorCode::E0003_INTERNAL_COMPILER_ERROR);
            FileTracker::AddError(binop->Token().file);
            return nullptr;
    }
}

llvm::Value* Venera::CodeGeneration::IRGenerator::GenerateUnaryOperation(const std::shared_ptr<Parsing::UnaryOp>& unaryop)
{
    llvm::Value* expr = GenerateExpression(unaryop->GetExpression());
    if (!expr)
        return nullptr;

    if (!unaryop->ResolveType().IsNumeric())
    {
        Error::Report("Cannot generate expression", unaryop->Token(), ErrorCode::E0003_INTERNAL_COMPILER_ERROR);
        FileTracker::AddError(unaryop->Token().file);
        return nullptr;
    }

    return mBuilder->CreateNeg(expr, "neg");
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
    // The C runtime calls 'main', so the language's 'Main' is emitted under that symbol
    std::string symbolName = func->Name().lexeme == "Main" ? "main" : std::string(func->Name().lexeme);
    llvm::Function* fn = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, symbolName, mModule.get());

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

void Venera::CodeGeneration::IRGenerator::PrintIR() const
{
    mModule->print(llvm::outs(), nullptr);
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

bool Venera::CodeGeneration::IRGenerator::EmitObject(const std::string& path)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    llvm::Triple triple(llvm::sys::getDefaultTargetTriple());
    mModule->setTargetTriple(triple);

    std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(triple, error);
    if (!target)
    {
        llvm::errs() << error << "\n";
        return false;
    }

    llvm::TargetOptions options;
    std::unique_ptr<llvm::TargetMachine> machine(target->createTargetMachine(triple, "generic", "", options, llvm::Reloc::PIC_));

    mModule->setDataLayout(machine->createDataLayout());

    std::error_code ec;
    llvm::raw_fd_ostream out(path, ec, llvm::sys::fs::OF_None);
    if (ec)
    {
        llvm::errs() << "Could not open file: " << ec.message() << "\n";
        return false;
    }

    llvm::legacy::PassManager pass;
    if (machine->addPassesToEmitFile(pass, out, nullptr, llvm::CodeGenFileType::ObjectFile))
    {
        llvm::errs() << "Target can't emit an object file\n";
        return false;
    }

    pass.run(*mModule);
    out.flush();
    return true;
}

Venera::CodeGeneration::IRGenerator::IRGenerator(std::shared_ptr<Parsing::Program> ast) :
    mAst(std::move(ast)),
    mContext(std::make_unique<llvm::LLVMContext>()),
    mModule(std::make_unique<llvm::Module>("<venera>", *mContext)),
    mBuilder(std::make_unique<llvm::IRBuilder<>>(*mContext)) {
}