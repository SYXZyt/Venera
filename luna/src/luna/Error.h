#pragma once
#include <vector>
#include <optional>
#include <string_view>
#include <luna/ErrorCode.h>
#include <luna/Lexing/Token.h>

namespace Luna
{
    class Error final
    {
    private:
        Error() = delete;

        static inline std::vector<ErrorCode> sErrorCodes = {};
        static inline bool sFailed = false;

    public:
        static const std::vector<ErrorCode>& ErrorCodes() {
            return sErrorCodes;
        }

        static bool Failed() {
            return sFailed;
        }

        static void Clear();

        static void PrintErrorCodes();

        static void Report(std::string_view message, std::optional<Lexing::Token> token = std::nullopt, ErrorCode code = ErrorCode::E0001_GENERIC_ERROR);
        static void ReportFailedFile();
    };
}