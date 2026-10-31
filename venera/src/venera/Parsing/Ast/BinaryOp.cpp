#include "BinaryOp.h"
#include <print>
#include <string>

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

    return lhs.IsNumeric() && rhs.IsNumeric() ? LanguageType::CommonNumericType(lhs, rhs) : (lhs == rhs ? lhs : LanguageType::Runtime);
}