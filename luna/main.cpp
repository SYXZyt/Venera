#include <iostream>
#include <string>
#include <print>

std::string gInput = R"(PROGRAM DO
    NAME DO
        I32 x
    END
END
)";

int main(int, char**)
{
    std::println("{}\n", gInput);
}