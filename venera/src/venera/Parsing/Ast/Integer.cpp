#include "Integer.h"
#include <print>

void Venera::Parsing::Integer::Dump(int indent) {
    std::println("{}<INTEGER {}>", std::string(indent, ' '), mValue);
}