cmake_minimum_required(VERSION 3.10)
project(Venera)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(venera main.cpp
        src/venera/Type.h
        src/venera/Lexing/TokenType.h
        src/venera/Lexing/TokenType.cpp
        src/venera/File.h
        src/venera/File.cpp
        src/venera/Helpers.h
        src/venera/Helpers.cpp
        src/venera/Lexing/Token.h
        src/venera/Lexing/Tokeniser.h
        src/venera/ANSI.cpp
        src/venera/FileTracker.h
        src/venera/FileTracker.cpp
        src/venera/ErrorCode.h
        src/venera/ErrorCode.cpp
        src/venera/Error.cpp
        src/venera/Lexing/Tokeniser.cpp
 "src/venera/Parsing/Statement.h" "src/venera/Parsing/Ast/Program.h" "src/venera/Parsing/Ast/Program.cpp" "src/venera/Parsing/Expression.h" "src/venera/Parsing/Ast/FunctionDefinition.h" "src/venera/Parsing/Ast/Block.h" "src/venera/Parsing/Ast/Block.cpp" "src/venera/LanguageType.h" "src/venera/LanguageType.cpp" "src/venera/Parsing/Ast/FunctionDefinition.cpp" "src/venera/Parsing/Ast/Integer.h" "src/venera/Parsing/Ast/Integer.cpp" "src/venera/Parsing/Ast/Return.h" "src/venera/Parsing/Ast/Return.cpp" "src/venera/Parsing/Ast.h" "src/venera/Parsing/Parser.h" "src/venera/Parsing/Parser.cpp")

target_include_directories(venera PUBLIC src)