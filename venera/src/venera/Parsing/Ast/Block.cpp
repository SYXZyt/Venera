#include "Block.h"
#include <print>

void Venera::Parsing::Block::Dump(int indent)
{
    std::println("{}<BLOCK>", std::string(indent, ' '));

    for (const auto& stmt : mStatements)
        stmt->Dump(indent + 2);
}