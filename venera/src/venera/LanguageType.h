#pragma once
#include <venera/Type.h>
#include <venera/Lexing/Token.h>
#include <string>

namespace Venera
{
    enum class LanguageTypeKind : uint8
    {
        INVALID,
        PRIMITIVE,
        FUNCTION,
    };

    struct LanguageType final
    {
        bool operator==(const LanguageType& other) const;
        bool operator!=(const LanguageType& other) const {
            return !(*this == other);
        }

        static LanguageType Runtime;
        static LanguageType Int32;
        static LanguageType Int64;
        static LanguageType UInt32;
        static LanguageType UInt64;
        static LanguageType Float32;
        static LanguageType Float64;

        LanguageTypeKind kind = LanguageTypeKind::INVALID;
        Lexing::TokenType primitiveType = Lexing::TokenType::NONE;
        uint8 ptr = 0;

        std::string ToString() const;

        static bool CanCoerce(const LanguageType& a, const LanguageType& b);

        bool IsUnsigned() const;
        bool IsNumeric() const;

        bool FitsUnsignedValue(uint64 value) const;

        static LanguageType CommonNumericType(const LanguageType& a, const LanguageType& b);
    };
}