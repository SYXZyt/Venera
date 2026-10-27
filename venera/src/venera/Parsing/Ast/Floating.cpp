#include "Floating.h"
#include <print>

void Venera::Parsing::Floating::Dump(int indent) {
    std::println("{}<Floating {}>", std::string(indent, ' '), mValue);
}