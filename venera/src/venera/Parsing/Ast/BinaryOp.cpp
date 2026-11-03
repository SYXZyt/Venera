#include "BinaryOp.h"
#include <print>
#include <string>
#include <venera/Helpers.h>

bool Venera::Parsing::BinaryOp::IsComparison() const
{
    return IsOneOf(mOp.type,
        Lexing::TokenType::OP_EQ, Lexing::TokenType::OP_NEQ,
        Lexing::TokenType::OP_GRT, Lexing::TokenType::OP_GRE,
        Lexing::TokenType::OP_LSS, Lexing::TokenType::OP_LSE);
}

void Venera::Parsing::BinaryOp::Dump(int indent)
{
    std::println("{}<BINARY OP {}>", std::string(indent, ' '), mOp.lexeme);
    mLhs->Dump(indent + 4);
    mRhs->Dump(indent + 4);
}

Venera::LanguageType Venera::Parsing::BinaryOp::ResolveType()
{
    if (IsComparison())
        return LanguageType::Boolean;

    LanguageType lhs = mLhs->ResolveType();
    LanguageType rhs = mRhs->ResolveType();

    if (lhs == LanguageType::Runtime || rhs == LanguageType::Runtime)
        return LanguageType::Runtime;

    return lhs.IsNumeric() && rhs.IsNumeric() ? LanguageType::CommonNumericType(lhs, rhs) : (lhs == rhs ? lhs : LanguageType::Runtime);
}