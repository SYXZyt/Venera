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

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

std::string gInput = R"(
PROGRAM DO
    DEF I32 Main DO
        RETURN 0
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

    Venera::Error::ReportFailedFile();
    if (Venera::Error::Failed())
        return 3;

    llvm::LLVMContext context;
    llvm::Module module("venera", context);
    module.print(llvm::outs(), nullptr);

    return 0;
}