#include "FunctionDefinition.h"
#include <print>

void Venera::Parsing::FunctionDefinition::Dump(int indent)
{
    std::println("{}<FUNCTION DEFINITION {} -> >", std::string(indent, ' '), mName.lexeme, mReturnType.ToString());
    mBlock->Dump(indent + 2);
}