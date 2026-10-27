#include "Analyser.h"
#include <venera/Error.h>
#include <venera/FileTracker.h>
#include <format>

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

    const LanguageType& expected = mCurrentFunction->ReturnType();

    if (TryCoerceLiteral(value, expected))
        return;

    LanguageType type = value->ResolveType();
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