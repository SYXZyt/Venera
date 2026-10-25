#include "Error.h"
#include <set>
#include <vector>
#include <print>
#include <luna/ANSI.h>
#include <luna/FileTracker.h>

static bool IsErrorCodeWarning(Luna::ErrorCode code) {
    return Luna::ToString(code)[0] == 'W';
}

void Luna::Error::Clear()
{
    sErrorCodes.clear();
    sFailed = false;
}

void Luna::Error::PrintErrorCodes()
{
    std::set<ErrorCode> printedCodes;

    // Make sure we don't print the same code twice
    for (ErrorCode code : sErrorCodes)
        printedCodes.insert(code);

    for (ErrorCode code : printedCodes)
    {
        std::string codeString = std::string(ToString(code));

        if (IsErrorCodeWarning(code))
            std::print("{}{}{} ", ANSI::Colours::Yellow, codeString, ANSI::Reset());
        else
            std::print("{}{}{} ", ANSI::Colours::Red, codeString, ANSI::Reset());
    }

    if (!sErrorCodes.empty())
        std::println();
}

void Luna::Error::Report(std::string_view message, std::optional<Lexing::Token> token, ErrorCode code)
{
    bool isWarning = IsErrorCodeWarning(code);
    if (!isWarning)
        sFailed = true;

    sErrorCodes.push_back(code);

    if (isWarning)
        std::println("{}Warning{}: {}{}", ANSI::Colours::Yellow, ANSI::Reset(), message, ANSI::Reset());
    else
        std::println("{}Error{}: {}{}", ANSI::Colours::Red, ANSI::Reset(), message, ANSI::Reset());

    if (!token)
        return;

    const Lexing::Token& t = *token;

    int lineBefore = t.line - 1;
    int lineAfter = t.line + 1;

    if (lineBefore >= 0)
        std::println("{}{:5}{}| {}{}{}", ANSI::Colours::Purple, lineBefore + 1, ANSI::Reset(), ANSI::Colours::Grey, t.file->Lines()[lineBefore], ANSI::Reset());

    std::println("{}{:5}{}| {}", ANSI::Colours::Purple, t.line + 1, ANSI::Reset(), t.file->Lines()[t.line]);

    int posOfCaret = t.col;

    // Line number will be 5 chars, then one for the pipe and then a space so 7 chars padding
    constexpr int padding = 7;
    const std::string& underlineColour = isWarning ? ANSI::Colours::Yellow : ANSI::Colours::Red;

    std::println("{}{}{}{}", std::string(padding + posOfCaret, ' '), underlineColour, std::string(t.lexeme.size(), '~'), ANSI::Reset());
    std::println("{}{}{}{}", std::string(padding + posOfCaret, ' '), ANSI::Colours::Aqua, std::string(t.lexeme.size(), '^'), ANSI::Reset());

    if (lineAfter < t.file->Lines().size())
        std::println("{}{:5}{}| {}{}{}", ANSI::Colours::Purple, lineAfter + 1, ANSI::Reset(), ANSI::Colours::Grey, t.file->Lines()[lineAfter], ANSI::Reset());

    std::println("Occurred at line {}{}{}, column {}{}{} in '{}{}{}'\n", ANSI::Colours::Purple, t.line + 1, ANSI::Reset(), ANSI::Colours::Purple, t.col + 1, ANSI::Reset(), ANSI::Colours::Yellow, t.file->Name(), ANSI::Reset());
}

void Luna::Error::ReportFailedFile()
{
    auto PrintSuccess = []() { std::println("{}Build successful{}", ANSI::Colours::Green, ANSI::Reset()); };

    std::vector<std::shared_ptr<File>> failed = FileTracker::GetErroredOrWarnedFiles();
    if (failed.empty())
    {
        PrintSuccess();
        return;
    }

    int totalErrors = 0;

    for (const std::shared_ptr<File>& file : failed)
    {
        auto [errors, warnings] = FileTracker::GetCount(file);
        totalErrors += errors;

        if (warnings > 0)
            std::println("{}{} warning{} in {}{}{}{}", ANSI::Colours::Yellow, warnings, (warnings != 1 ? "s" : ""), ANSI::Reset(), ANSI::Colours::Yellow, file->Name(), ANSI::Reset());

        if (errors > 0)
            std::println("{}{} error{} in {}{}{}{}", ANSI::Colours::Red, errors, (errors != 1 ? "s" : ""), ANSI::Reset(), ANSI::Colours::Red, file->Name(), ANSI::Reset());
    }

    if (totalErrors == 0)
        PrintSuccess();

    PrintErrorCodes();
}