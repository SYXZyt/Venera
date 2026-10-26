#pragma once
#include <venera/Type.h>

// We have a lot of error codes and remembering to add it to the `ToString` is a pain
// So we can automate it with a python script
namespace Venera
{
    enum class ErrorCode : uint16
    {
        // Generic
        E0000_NO_ERROR,
        E0001_GENERIC_ERROR,
        E0002_EMPTY_FILE,
        E0003_INTERNAL_COMPILER_ERROR,

        E1000_ILLEGAL_CHARACTER,
        E1001_EXPECTED_NUMERIC_AFTER_HEX,
        E1002_EXPECTED_EQUALS,
        E1003_UNKNOWN_ESCAPE_SEQUENCE,
        E1004_UNTERMINATED_STRING,
        E1005_UNTERMINATED_CHAR,

        E2000_UNEXPECTED_EOF,
        E2001_NO_PROGRAM,
        E2002_NO_DO,
        E2003_EXPECTED_BLOCK,
        E2004_EXPECTED_STATEMENT,
        E2005_EXPECTED_EXPRESSION,
        E2006_INTEGER_LITERAL_OUT_OF_RANGE,
        E2007_EXPECTED_TYPE,
        E2008_EXPECTED_IDENTIFIER,
        E2009_EXPECTED_END,
    };

    extern const char* ToString(ErrorCode code);
}