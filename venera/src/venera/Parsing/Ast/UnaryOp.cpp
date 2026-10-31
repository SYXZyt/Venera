#include "UnaryOp.h"
#include <venera/Error.h>
#include <venera/FileTracker.h>
#include <print>
#include <string>
#include <format>

void Venera::Parsing::UnaryOp::Dump(int indent)
{
    std::println("{}<UNARY OP {}>", std::string(indent, ' '), mToken.lexeme);
    mExpr->Dump(indent + 4);
}

Venera::LanguageType Venera::Parsing::UnaryOp::ResolveType() {
    return mExpr->ResolveType();
}