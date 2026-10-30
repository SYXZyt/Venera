#pragma once
#include <vector>
#include <initializer_list>
#include <string>
#include <string_view>
#include <venera/Lexing/Token.h>
#include <venera/LanguageType.h>
#include <venera/Parsing/Ast.h>
#include <venera/ErrorCode.h>
#include <memory>
#include <optional>
#include <tuple>

namespace Venera::Parsing
{
    class Parser final
    {
    private:
        std::vector<Lexing::Token> mTokens;
        size_t mCurrent;

        const Lexing::Token& Current() const;
        const Lexing::Token& Last() const;

        std::shared_ptr<File> OwnFile() const {
            return mTokens.front().file;
        }

        void Sync();

        bool CheckEof(std::string_view customErrorMessage = "") const;

        bool Check(Lexing::TokenType type) const {
            return Check({ type });
        }

        bool Check(std::initializer_list<Lexing::TokenType> types) const;

        bool Accept(Lexing::TokenType type) {
            return Accept({ type });
        }

        bool Accept(std::initializer_list<Lexing::TokenType> types);

        bool Expect(Lexing::TokenType type, std::string_view errorMessage = "", ErrorCode errorCode = ErrorCode::E0001_GENERIC_ERROR) {
            return Expect({ type }, errorMessage, errorCode);
        }

        bool Expect(std::initializer_list<Lexing::TokenType> types, std::string_view errorMessage = "", ErrorCode errorCode = ErrorCode::E0001_GENERIC_ERROR);

        bool CheckType();

        bool ParseFunctionSig(Lexing::Token& outName, LanguageType& outReturnType);

        std::tuple<std::string /* digits */, bool /* isUnsigned */, bool /* isForced64 */ > SplitIntegerSuffix(const std::string& lexeme);

        std::shared_ptr<Integer> ParseBase10();
        std::shared_ptr<Floating> ParseFloating();

        std::shared_ptr<Expression> ParsePrimary();
        std::shared_ptr<Expression> ParseFactor();
        std::shared_ptr<Expression> ParseTerm();
        std::shared_ptr<Expression> ParseArithmetic();

        std::shared_ptr<Expression> ParseExpression();

        std::optional<LanguageType> ParseType();

        std::shared_ptr<Block> ParseBlock();
        std::shared_ptr<Statement> ParseStatement();

        std::shared_ptr<FunctionDefinition> ParseFunctionDef();

    public:
        std::shared_ptr<Program> Parse();

        Parser(std::vector<Lexing::Token> tokens);
    };
}