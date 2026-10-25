#pragma once
#include <venera/Type.h>

namespace Venera::Lexing
{
    enum class TokenType : uint8
    {
        NONE,

        IDENTIFIER,

        KW_PROGRAM,
        KW_DO,
        KW_END,
        KW_NAME,

        TY_U8,
        TY_U16,
        TY_U32,
        TY_U64,

        TY_I8,
        TY_I16,
        TY_I32,
        TY_I64,
    };

    extern const char* ToString(TokenType type);
}

#include <format>

template <>
struct std::formatter<Venera::Lexing::TokenType>
{
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(Venera::Lexing::TokenType type, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", Venera::Lexing::ToString(type));
    }
};