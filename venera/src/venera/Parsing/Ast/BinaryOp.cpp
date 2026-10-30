#include "BinaryOp.h"
#include <venera/Error.h>
#include <venera/FileTracker.h>
#include <print>
#include <string>
#include <format>

void Venera::Parsing::BinaryOp::Dump(int indent)
{
    std::println("{}<BINARY OP {}>", std::string(indent, ' '), mOp.lexeme);
    mLhs->Dump(indent + 4);
    mRhs->Dump(indent + 4);
}

Venera::LanguageType Venera::Parsing::BinaryOp::ResolveType()
{
    LanguageType lhs = mLhs->ResolveType();
    LanguageType rhs = mRhs->ResolveType();

    if (lhs == LanguageType::Runtime || rhs == LanguageType::Runtime)
        return LanguageType::Runtime;

    LanguageType common = lhs.IsNumeric() && rhs.IsNumeric() ? LanguageType::CommonNumericType(lhs, rhs) : (lhs == rhs ? lhs : LanguageType::Runtime);

    if (common == LanguageType::Runtime)
    {
        Error::Report(std::format("Cannot implicitly combine operand types '{}' and '{}'", lhs.ToString(), rhs.ToString()), mOp, ErrorCode::E3006_INCOMPATIBLE_OPERAND_TYPES);
        FileTracker::AddError(mOp.file);
    }

    return common;
}