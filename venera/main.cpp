#include <iostream>
#include <string>
#include <print>

#include <venera/Lexing/TokenType.h>
#include <venera/Lexing/Token.h>
#include <venera/File.h>
#include <venera/FileTracker.h>
#include <venera/Error.h>
#include <venera/Lexing/Tokeniser.h>
#include <venera/Parsing/Parser.h>
#include <venera/Analysis/Analyser.h>
#include <venera/CodeGeneration/IRGenerator.h>
#include <venera/Linker.h>
#include <venera/ANSI.h>

std::string gInput = R"(
PROGRAM DO
    DEF I32 Main DO
        RETURN 2147483648
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

    Venera::Parsing::Parser parser(lexer.GetTokens());
    std::shared_ptr<Venera::Parsing::Program> program = parser.Parse();

    if (Venera::Error::Failed() || !program)
    {
        Venera::Error::ReportFailedFile();
        return 2;
    }

    program->Dump(0);

    Venera::Analysis::Analyser analyser(program);
    analyser.Analyse();

    if (Venera::Error::Failed())
    {
        Venera::Error::ReportFailedFile();
        return 3;
    }

    Venera::CodeGeneration::IRGenerator irGen(program);
    irGen.Generate();

    std::string outputPath = "prog"
#ifdef _WIN32
        ".exe"
#endif
    ;

    if (!irGen.EmitObject("prog.o") || !Venera::Linker::Link("prog.o", outputPath))
    {
        Venera::Error::ReportFailedFile();
        return 4;
    }

    std::println("{}Built {}{}", Venera::ANSI::Colours::Green, outputPath, Venera::ANSI::Reset());

    Venera::Error::ReportFailedFile();
    return 0;
}