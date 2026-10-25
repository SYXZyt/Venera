cmake_minimum_required(VERSION 3.10)
project(Venera)

add_executable(luna main.cpp
        src/luna/Type.h
        src/luna/Lexing/TokenType.h
        src/luna/Lexing/TokenType.cpp
        src/luna/File.h
        src/luna/File.cpp
        src/luna/Helpers.h
        src/luna/Helpers.cpp
        src/luna/Lexing/Token.h
        src/luna/Lexing/Tokeniser.h
        src/luna/ANSI.cpp
        src/luna/FileTracker.h
        src/luna/FileTracker.cpp
        src/luna/ErrorCode.h
        src/luna/ErrorCode.cpp
        src/luna/Error.cpp
)

target_include_directories(luna PUBLIC src)