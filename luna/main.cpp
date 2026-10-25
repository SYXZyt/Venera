#include <iostream>
#include <string>
#include <print>

#include "luna/Lexing/TokenType.h"

std::string gInput = R"(PROGRAM DO
    NAME DO
        I32 x
    END
END
)";

int main(int, char**)
{
    std::println("{}\n", gInput);

    std::println("{}\n", Luna::Lexing::TokenType::IDENTIFIER);
}