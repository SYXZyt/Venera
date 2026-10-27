#include "Parser.h"
#include <venera/Error.h>
#include <venera/FileTracker.h>
#include <venera/Helpers.h>
#include <format>

const Venera::Lexing::Token& Venera::Parsing::Parser::Current() const
{
    if (mCurrent < mTokens.size())
        return mTokens.at(mCurrent);

    return mTokens.back();
}

const Venera::Lexing::Token& Venera::Parsing::Parser::Last() const
{
    if (mCurrent == 0)
        return mTokens.front();

    if (mCurrent - 1ull < mTokens.size())
        return mTokens.at(mCurrent - 1ull);

    return mTokens.back();
}

void Venera::Parsing::Parser::Sync()
{
    int scope = 0;

    while (mCurrent < mTokens.size())
    {
        if (Current().type == Lexing::TokenType::KW_DO)
            ++scope;

        if (Current().type == Lexing::TokenType::KW_END)
        {
            if (scope == 0)
            {
                ++mCurrent;
                return;
            }

            --scope;
        }

        ++mCurrent;
    }
}

bool Venera::Parsing::Parser::CheckEof(std::string_view customErrorMessage) const
{
    if (mCurrent >= mTokens.size())
    {
        Error::Report(customErrorMessage.size() == 0 ? "Unexpected end of file" : customErrorMessage, mTokens.back(), ErrorCode::E2000_UNEXPECTED_EOF);
        FileTracker::AddError(OwnFile());
        return false;
    }

    return true;
}

bool Venera::Parsing::Parser::Check(std::initializer_list<Lexing::TokenType> types) const
{
    if (mCurrent >= mTokens.size())
        return false;

    for (Lexing::TokenType type : types)
        if (Current().type == type)
            return true;

    return false;
}

bool Venera::Parsing::Parser::Accept(std::initializer_list<Lexing::TokenType> types)
{
    if (!Check(types))
        return false;

    ++mCurrent;
    return true;
}

bool Venera::Parsing::Parser::Expect(std::initializer_list<Lexing::TokenType> types, std::string_view errorMessage, ErrorCode errorCode)
{
    if (Accept(types))
        return true;

    Error::Report(errorMessage.size() ? errorMessage : "Unexpected token", Current(), errorCode);
    FileTracker::AddError(OwnFile());
    Sync();

    return false;
}

bool Venera::Parsing::Parser::CheckType()
{
    switch (Current().type)
    {
        case Lexing::TokenType::TY_U8:
        case Lexing::TokenType::TY_U16:
        case Lexing::TokenType::TY_U32:
        case Lexing::TokenType::TY_U64:
        case Lexing::TokenType::TY_I8:
        case Lexing::TokenType::TY_I16:
        case Lexing::TokenType::TY_I32:
        case Lexing::TokenType::TY_I64:
            return true;

        default:
            return false;
    }
}

bool Venera::Parsing::Parser::ParseFunctionSig(Lexing::Token& outName, LanguageType& outReturnType)
{
    std::optional parsedReturnType = ParseType();
    if (!parsedReturnType)
        return false;

    outReturnType = *parsedReturnType;

    if (!Expect(Lexing::TokenType::IDENTIFIER, "Expected function name", ErrorCode::E2008_EXPECTED_IDENTIFIER))
        return false;

    outName = Last();

    return true;
}

std::tuple<std::string, bool, bool> Venera::Parsing::Parser::SplitIntegerSuffix(const std::string& lexeme)
{
    if (lexeme.ends_with("ul") || lexeme.ends_with("lu"))
        return { lexeme.substr(0, lexeme.size() - 2), true, true };

    if (lexeme.ends_with("u"))
        return { lexeme.substr(0, lexeme.size() - 1), true, false };

    if (lexeme.ends_with("l"))
        return { lexeme.substr(0, lexeme.size() - 1), false, true };

    return { lexeme, false, false };
}

std::shared_ptr<Venera::Parsing::Integer> Venera::Parsing::Parser::ParseBase10()
{
    Lexing::Token token = Last();
    auto [digits, isUnsigned, isLong] = SplitIntegerSuffix(token.lexeme);

    uint64 raw;
    if (!TryParseInt(digits, raw))
    {
        Error::Report(std::format("Integer literal '{}' is out of range", token.lexeme), token, ErrorCode::E2006_INTEGER_LITERAL_OUT_OF_RANGE);
        FileTracker::AddError(token.file);
        Sync();

        return nullptr;
    }

    if (isUnsigned)
    {
        LanguageType unsignedType = isLong || raw > UINT32_MAX ? LanguageType::UInt64 : LanguageType::UInt32;
        return std::make_shared<Integer>(token, (int64)raw, unsignedType);
    }

    if (raw > INT64_MAX)
    {
        Error::Report(std::format("Integer literal '{}' is out of range", token.lexeme), token, ErrorCode::E2006_INTEGER_LITERAL_OUT_OF_RANGE);
        FileTracker::AddError(token.file);
        Sync();

        return nullptr;
    }

    LanguageType type = isLong || raw > UINT32_MAX ? LanguageType::Int64 : LanguageType::Int32;
    return std::make_shared<Integer>(token, (int64)raw, type);
}

std::shared_ptr<Venera::Parsing::Expression> Venera::Parsing::Parser::ParsePrimary()
{
    if (Accept(Lexing::TokenType::LIT_BASE10))
        return ParseBase10();

    Error::Report("Expected expression", Current(), ErrorCode::E2005_EXPECTED_EXPRESSION);
    FileTracker::AddError(Current().file);
    Sync();
    return nullptr;
}

std::shared_ptr<Venera::Parsing::Expression> Venera::Parsing::Parser::ParseExpression()
{
    return ParsePrimary();
}

std::optional<Venera::LanguageType> Venera::Parsing::Parser::ParseType()
{
    if (!CheckType())
    {
        Error::Report("Expected type", Current(), ErrorCode::E2007_EXPECTED_TYPE);
        FileTracker::AddError(Current().file);
        Sync();
        return std::nullopt;
    }

    LanguageType type;

    if (Current().type == Lexing::TokenType::IDENTIFIER)
    {
        Error::Report("Expected type", Current(), ErrorCode::E2007_EXPECTED_TYPE);
        FileTracker::AddError(Current().file);
        Sync();
        return std::nullopt;
    }
    else
    {
        ++mCurrent;

        LanguageTypeKind kind = LanguageTypeKind::PRIMITIVE;
        type = LanguageType
        {
            .kind = kind,
            .primitiveType = Last().type,
            .ptr = 0,
        };
    }

    if (mCurrent >= mTokens.size())
        return type;

    return type;
}

std::shared_ptr<Venera::Parsing::Block> Venera::Parsing::Parser::ParseBlock()
{
    if (!Expect(Lexing::TokenType::KW_DO, "Expected 'DO'", ErrorCode::E2003_EXPECTED_BLOCK))
        return nullptr;

    Lexing::Token blockBegin = Current();

    std::vector<std::shared_ptr<Statement>> statements;
    while (mCurrent <= mTokens.size() && Current().type != Lexing::TokenType::KW_END)
    {
        std::shared_ptr stmt = ParseStatement();
        if (!stmt)
            break;

        statements.push_back(std::move(stmt));
    }

    if (!Expect(Lexing::TokenType::KW_END, "Expected 'END'", ErrorCode::E2003_EXPECTED_BLOCK))
        return nullptr;

    return std::make_shared<Block>(blockBegin, std::move(statements));
}

std::shared_ptr<Venera::Parsing::Statement> Venera::Parsing::Parser::ParseStatement()
{
    if (Accept(Lexing::TokenType::KW_RETURN))
    {
        Lexing::Token returnToken = Last();
        std::shared_ptr returnValue = ParseExpression();
        return std::make_shared<Return>(returnValue, returnToken);
    }
    else if (Check(Lexing::TokenType::KW_DO))
    {
        return ParseBlock();
    }
    else
    {
        Error::Report("Expected statement", Current(), ErrorCode::E2004_EXPECTED_STATEMENT);
        FileTracker::AddError(Current().file);
        Sync();
        return nullptr;
    }
}

std::shared_ptr<Venera::Parsing::FunctionDefinition> Venera::Parsing::Parser::ParseFunctionDef()
{
    Lexing::Token def = Current();
    ++mCurrent;

    Lexing::Token funcName;
    LanguageType returnType;

    if (!ParseFunctionSig(funcName, returnType))
        return nullptr;

    std::shared_ptr block = ParseBlock();
    if (!block)
        return nullptr;

    return std::make_shared<FunctionDefinition>(block, returnType, funcName, def);
}

std::shared_ptr<Venera::Parsing::Program> Venera::Parsing::Parser::Parse()
{
    if (!Expect(Lexing::TokenType::KW_PROGRAM, "Expected 'PROGRAM'", ErrorCode::E2001_NO_PROGRAM))
        return nullptr;

    std::vector<std::shared_ptr<Statement>> programCode;

    Lexing::Token program = Last();
    std::optional<Lexing::Token> name;

    if (Accept(Lexing::TokenType::IDENTIFIER))
        name = Last();

    if (!Expect(Lexing::TokenType::KW_DO, "Expected 'DO'", ErrorCode::E2002_NO_DO))
        return nullptr;

    while (mCurrent < mTokens.size() && Current().type != Lexing::TokenType::KW_END)
    {
        std::shared_ptr declaration = ParseFunctionDef();
        if (!declaration)
            return nullptr;

        programCode.push_back(declaration);
    }

    if (!Expect(Lexing::TokenType::KW_END, "Expected 'END'", ErrorCode::E2009_EXPECTED_END))
        return nullptr;

    return std::make_shared<Program>(programCode, name, program);
}

Venera::Parsing::Parser::Parser(std::vector<Lexing::Token> tokens) :
    mTokens(std::move(tokens)),
    mCurrent(0) {
}