#include "Return.h"
#include <print>

void Venera::Parsing::Return::Dump(int indent)
{
    std::println("{}<RETURN>", std::string(indent, ' '));
    mReturnValue->Dump(indent + 2);
}