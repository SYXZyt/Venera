#pragma once
#include <luna/Type.h>

// We have alot of error codes and remembering to add it to the `ToString` is a pain
// So we can automate it with a python script
namespace Luna
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
    };

    extern const char* ToString(ErrorCode code);
}

#undef PY_AUTOGEN_BEGIN