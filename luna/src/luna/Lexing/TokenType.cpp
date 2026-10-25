#include "TokenType.h"

#define TOKEN(type) case TokenType::type: return #type

const char* Luna::Lexing::TokenToString(TokenType type)
{
    switch (type)
    {
        TOKEN(NONE);
        TOKEN(IDENTIFIER);
        TOKEN(KW_PROGRAM);
        TOKEN(KW_DO);
        TOKEN(KW_END);
        TOKEN(KW_NAME);
        TOKEN(TY_U8);
        TOKEN(TY_U16);
        TOKEN(TY_U32);
        TOKEN(TY_U64);
        TOKEN(TY_I8);
        TOKEN(TY_I16);
        TOKEN(TY_I32);
        TOKEN(TY_I64);
        default: return "UNKNOWN";
    }
}