#pragma once
#include <venera/Type.h>

namespace Venera::Lexing
{
    enum class TokenType : uint8
    {
        NONE,

        IDENTIFIER,

        L_PAREN,
        R_PAREN,

        KW_PROGRAM,
        KW_DO,
        KW_END,
        KW_NAME,
        KW_RETURN,

        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,

        TY_U8,
        TY_U16,
        TY_U32,
        TY_U64,

        TY_I8,
        TY_I16,
        TY_I32,
        TY_I64,

        TY_F32,
        TY_F64,

        LIT_BASE10,
        LIT_FLOATING,
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