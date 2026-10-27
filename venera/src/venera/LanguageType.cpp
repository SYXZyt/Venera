#include "LanguageType.h"
#include <sstream>
#include <venera/Helpers.h>

bool Venera::LanguageType::operator==(const LanguageType& other) const
{
    return
        kind == other.kind &&
        primitiveType == other.primitiveType &&
        ptr == other.ptr;
}

using Venera::Lexing::TokenType;
using Venera::LanguageTypeKind;

Venera::LanguageType Venera::LanguageType::Runtime = { .kind = LanguageTypeKind::INVALID };
Venera::LanguageType Venera::LanguageType::Int32 = { .kind = LanguageTypeKind::PRIMITIVE, .primitiveType = TokenType::TY_I32 };
Venera::LanguageType Venera::LanguageType::Int64 = { .kind = LanguageTypeKind::PRIMITIVE, .primitiveType = TokenType::TY_I64 };
Venera::LanguageType Venera::LanguageType::UInt32 = { .kind = LanguageTypeKind::PRIMITIVE, .primitiveType = TokenType::TY_U32 };
Venera::LanguageType Venera::LanguageType::UInt64 = { .kind = LanguageTypeKind::PRIMITIVE, .primitiveType = TokenType::TY_U64 };

std::string Venera::LanguageType::ToString() const
{
    std::stringstream ss;

    switch (kind)
    {
        case LanguageTypeKind::PRIMITIVE:
        {
            switch (primitiveType)
            {
                case TokenType::TY_U8:
                    ss << "U8";
                    break;
                case TokenType::TY_U16:
                    ss << "U16";
                    break;
                case TokenType::TY_U32:
                    ss << "U32";
                    break;
                case TokenType::TY_U64:
                    ss << "U64";
                    break;
                case TokenType::TY_I8:
                    ss << "I8";
                    break;
                case TokenType::TY_I16:
                    ss << "I16";
                    break;
                case TokenType::TY_I32:
                    ss << "I32";
                    break;
                case TokenType::TY_I64:
                    ss << "I64";
                    break;
                default:
                    ss << "UNKNOWN";
                    break;
            }

            break;
        }
    }

    int p = ptr;
    while (p > 0)
    {
        ss << "PTR";

        if (p > 1)
            ss << ' ';

        --p;
    }

    return ss.str();
}

bool Venera::LanguageType::IsUnsigned() const
{
    if (kind == LanguageTypeKind::PRIMITIVE)
        return IsOneOf(primitiveType, Lexing::TokenType::TY_U8, Lexing::TokenType::TY_U16, Lexing::TokenType::TY_U32, Lexing::TokenType::TY_U64);

    return false;
}