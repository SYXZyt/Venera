#include "LanguageType.h"
#include <sstream>

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



    return ss.str();
}