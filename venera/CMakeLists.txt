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
)

target_include_directories(venera PUBLIC src)