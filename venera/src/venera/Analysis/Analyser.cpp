#include "Analyser.h"
#include <venera/Error.h>
#include <venera/FileTracker.h>
#include <format>

void Venera::Analysis::Analyser::AnalyseExpression(std::shared_ptr<Parsing::Expression> expression)
{
    if (std::shared_ptr binary = std::dynamic_pointer_cast<Parsing::BinaryOp>(expression))
    {
        AnalyseBinary(binary);
    }
    else if (std::shared_ptr unary = std::dynamic_pointer_cast<Parsing::UnaryOp>(expression))
    {
        AnalyseUnary(unary);
    }
}

void Venera::Analysis::Analyser::AnalyseBinary(std::shared_ptr<Parsing::BinaryOp> binaryop)
{
    AnalyseExpression(binaryop->Left());
    AnalyseExpression(binaryop->Right());

    LanguageType lhs = binaryop->Left()->ResolveType();
    LanguageType rhs = binaryop->Right()->ResolveType();

    if (lhs == LanguageType::Runtime || rhs == LanguageType::Runtime)
        return;

    if (binaryop->ResolveType() == LanguageType::Runtime)
    {
        Error::Report(std::format("Cannot implicitly combine operand types '{}' and '{}'", lhs.ToString(), rhs.ToString()), binaryop->Operator(), ErrorCode::E3006_INCOMPATIBLE_OPERAND_TYPES);
        FileTracker::AddError(binaryop->Operator().file);
    }
}

void Venera::Analysis::Analyser::AnalyseUnary(std::shared_ptr<Parsing::UnaryOp> unaryop)
{
    AnalyseExpression(unaryop->GetExpression());

    LanguageType type = unaryop->GetExpression()->ResolveType();
    if (type == LanguageType::Runtime)
        return;

    if (!type.IsNumeric() || type.IsUnsigned())
    {
        Error::Report(std::format("Cannot negate a value of type {}", type.ToString()), unaryop->Token(), ErrorCode::E3007_INVALID_UNARY);
        FileTracker::AddError(unaryop->Token().file);
    }
}

void Venera::Analysis::Analyser::AnalyseFunction(std::shared_ptr<Parsing::FunctionDefinition> function)
{
    mCurrentFunction = function.get();
    if (!AnalyseBlock(function->GetBlock()))
    {
        Error::Report(std::format("Function '{}' does not return a value on all paths", function->Name().lexeme), function->Name(), ErrorCode::E3001_FUNCTION_NO_RETURN);
        FileTracker::AddError(function->Name().file);
    }

    mCurrentFunction = nullptr;
}

bool Venera::Analysis::Analyser::AnalyseBlock(std::shared_ptr<Parsing::Block> block)
{
    bool returns = false;

    for (const std::shared_ptr<Parsing::Statement>& stmt : block->Statements())
    {
        // Anything after a guaranteed return can never run, only report the first unreachable statement
        if (returns)
        {
            Error::Report("Unreachable code", stmt->Token(), ErrorCode::W3000_DEAD_CODE);
            FileTracker::AddWarning(stmt->Token().file);
            break;
        }

        if (std::shared_ptr ret = std::dynamic_pointer_cast<Parsing::Return>(stmt))
        {
            AnalyseReturn(ret);
            returns = true;
        }
        else if (std::shared_ptr inner = std::dynamic_pointer_cast<Parsing::Block>(stmt))
        {
            returns = AnalyseBlock(inner);
        }
    }

    return returns;
}

bool Venera::Analysis::Analyser::TryCoerceLiteral(const std::shared_ptr<Parsing::Expression>& value, const LanguageType& expected)
{
    std::shared_ptr literal = std::dynamic_pointer_cast<Parsing::Integer>(value);
    if (!literal)
        return false;

    if (!expected.FitsUnsignedValue((uint64)literal->Value()))
    {
        Error::Report(std::format("Literal '{}' does not fit in {}", literal->Value(), expected.ToString()), value->Token(), ErrorCode::E3004_LITERAL_DOES_NOT_FIT);
        FileTracker::AddError(value->Token().file);
        return true;
    }

    literal->SetType(expected);
    return true;
}

void Venera::Analysis::Analyser::AnalyseReturn(std::shared_ptr<Parsing::Return> ret)
{
    std::shared_ptr<Parsing::Expression> value = ret->ReturnValue();
    if (!value)
        return;

    AnalyseExpression(value);

    const LanguageType& expected = mCurrentFunction->ReturnType();

    if (TryCoerceLiteral(value, expected))
        return;

    LanguageType type = value->ResolveType();
    if (type == LanguageType::Runtime)
        return;

    if (type != expected)
    {
        Error::Report(std::format("Function '{}' returns {} but the return value is {}", mCurrentFunction->Name().lexeme, expected.ToString(), type.ToString()), value->Token(), ErrorCode::E3002_RETURN_TYPE_MISMATCH);
        FileTracker::AddError(value->Token().file);
    }
}

void Venera::Analysis::Analyser::Analyse()
{
    std::shared_ptr<Parsing::FunctionDefinition> main;

    for (const std::shared_ptr<Parsing::Statement>& stmt : mAst->Statements())
    {
        std::shared_ptr function = std::dynamic_pointer_cast<Parsing::FunctionDefinition>(stmt);
        if (!function)
            continue;

        if (function->Name().lexeme == "main")
        {
            Error::Report("Function cannot have the name 'main'", function->Token(), ErrorCode::E3005_CANNOT_CALL_MAIN);
            FileTracker::AddError(function->Token().file);
            continue;
        }

        if (function->Name().lexeme == "Main")
            main = function;

        AnalyseFunction(function);
    }

    if (!main)
    {
        Error::Report("Program has no 'Main' function", mAst->Token(), ErrorCode::E3003_NO_MAIN);
        FileTracker::AddError(mAst->Token().file);
        return;
    }

    if (main->ReturnType() != LanguageType::Int32)
    {
        Error::Report(std::format("'Main' must return I32, not {}", main->ReturnType().ToString()), main->Name(), ErrorCode::E3000_MAIN_NOT_I32);
        FileTracker::AddError(main->Name().file);
    }
}

Venera::Analysis::Analyser::Analyser(std::shared_ptr<Parsing::Program> ast) :
    mAst(std::move(ast)) {
}