#include "TokenType.h"

#define TOKEN(type) case TokenType::type: return #type

const char* Venera::Lexing::ToString(TokenType type)
{
    switch (type)
    {
        TOKEN(NONE);
        TOKEN(IDENTIFIER);
        TOKEN(L_PAREN);
        TOKEN(R_PAREN);
        TOKEN(KW_PROGRAM);
        TOKEN(KW_DO);
        TOKEN(KW_END);
        TOKEN(KW_NAME);
        TOKEN(KW_RETURN);
        TOKEN(OP_ADD);
        TOKEN(OP_SUB);
        TOKEN(OP_MUL);
        TOKEN(OP_DIV);
        TOKEN(TY_U8);
        TOKEN(TY_U16);
        TOKEN(TY_U32);
        TOKEN(TY_U64);
        TOKEN(TY_I8);
        TOKEN(TY_I16);
        TOKEN(TY_I32);
        TOKEN(TY_I64);
        TOKEN(TY_BOOL);
        TOKEN(LIT_BASE10);
        TOKEN(LIT_FLOATING);
        TOKEN(LIT_TRUE);
        TOKEN(LIT_FALSE);
        TOKEN(OP_EQ);
        TOKEN(OP_NEQ);
        TOKEN(OP_GRT);
        TOKEN(OP_GRE);
        TOKEN(OP_LSS);
        TOKEN(OP_LSE);
        default: return "UNKNOWN";
    }
}