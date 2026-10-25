#include <iostream>
#include <string>
#include <print>

#include <venera/Lexing/TokenType.h>
#include <venera/Lexing/Token.h>
#include <venera/File.h>
#include <venera/FileTracker.h>
#include <venera/Error.h>
#include <venera/Lexing/Tokeniser.h>

std::string gInput = R"(PROGRAM DO
    NAME DO
        I32 x
    END
END
)";

int main(int, char**)
{
    Venera::FileTracker::Clear();
    Venera::Error::Clear();

    Venera::Lexing::Tokeniser lexer(gInput, "<stdin>");
    lexer.Tokenise();

    if (Venera::Error::Failed())
    {
        Venera::Error::ReportFailedFile();
        return 1;
    }

    for (const Venera::Lexing::Token& t : lexer.GetTokens())
        std::println("{}", t);

    return 0;
}