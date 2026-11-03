#include "Boolean.h"
#include <print>

void Venera::Parsing::Boolean::Dump(int indent) {
    std::println("{}<BOOLEAN {}>", std::string(indent, ' '), mValue ? "TRUE" : "FALSE");
}