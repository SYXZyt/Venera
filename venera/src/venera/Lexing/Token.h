#pragma once
#include <venera/Lexing/TokenType.h>
#include <venera/File.h>
#include <string>
#include <memory>

namespace Venera::Lexing
{
    struct Token final
    {
        std::string lexeme;
        std::shared_ptr<File> file;
        int line;
        int col;
        TokenType type;
    };
}

#include <format>

template <>
struct std::formatter<Venera::Lexing::Token>
{
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Venera::Lexing::Token& token, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "<Token: '{}' at {}:{} in {} ({})>", token.lexeme, token.line + 1, token.col + 1, token.file->Name(), token.type);
    }
};