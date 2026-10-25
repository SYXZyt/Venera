#include <iostream>
#include <string>
#include <print>

#include <luna/Lexing/TokenType.h>
#include <luna/Lexing/Token.h>
#include <luna/File.h>
#include <luna/FileTracker.h>

std::string gInput = R"(PROGRAM DO
    NAME DO
        I32 x
    END
END
)";

int main(int, char**)
{
    Luna::FileTracker::Clear();

    std::shared_ptr file = Luna::File::FromSource("<stdin>", gInput);

    Luna::FileTracker::AddRegister(file);
}