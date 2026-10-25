#include "ANSI.h"
#include <format>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

bool Luna::ANSI::SupportsANSI()
{
#ifdef _WIN32
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode = 0;
    if (!GetConsoleMode(handle, &mode))
        return false;

    if ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0)
        return true;

    return SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);
#else
    return isatty(fileno(stdout));
#endif
}

const char* const Luna::ANSI::Reset()
{
    if (SupportsANSI())
        return "\033[0m";

    return "";
}

std::string Luna::ANSI::Colours::MakeIndexed(uint8 idx)
{
    if (!SupportsANSI())
        return "";

    return std::format("\033[38;5;{}m", idx);
}