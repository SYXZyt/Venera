#include "Program.h"
#include <print>

void Venera::Parsing::Program::Dump(int indent)
{
    std::println("{}<PROGRAM {}>", std::string(indent, ' '), mProgramName.has_value() ? mProgramName->lexeme : "unnamed");

    for (const auto& stmt : mStatements)
        stmt->Dump(indent + 2);
}