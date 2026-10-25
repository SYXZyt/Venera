#include <iostream>
#include <string>
#include <print>

#include <luna/Lexing/TokenType.h>
#include <luna/Lexing/Token.h>
#include <luna/File.h>
#include <luna/FileTracker.h>
#include <luna/Error.h>
#include <luna/Lexing/Tokeniser.h>

std::string gInput = R"(PROGRAM DO
    NAME DO
        I32 x
    END
END
)";

int main(int, char**)
{
    Luna::FileTracker::Clear();
    Luna::Error::Clear();

    Luna::Lexing::Tokeniser lexer(gInput, "<stdin>");
    lexer.Tokenise();

    if (Luna::Error::Failed())
    {
        Luna::Error::ReportFailedFile();
        return 1;
    }

    for (const Luna::Lexing::Token& t : lexer.GetTokens())
        std::println("{}", t);

    return 0;
}