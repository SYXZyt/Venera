cmake_minimum_required(VERSION 3.20)
project(Venera)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

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
        src/venera/Parsing/Statement.h
        src/venera/Parsing/Ast/Program.h
        src/venera/Parsing/Ast/Program.cpp
        src/venera/Parsing/Expression.h
        src/venera/Parsing/Ast/FunctionDefinition.h
        src/venera/Parsing/Ast/Block.h
        src/venera/Parsing/Ast/Block.cpp
        src/venera/LanguageType.h
        src/venera/LanguageType.cpp
        src/venera/Parsing/Ast/FunctionDefinition.cpp
        src/venera/Parsing/Ast/Integer.h
        src/venera/Parsing/Ast/Integer.cpp
        src/venera/Parsing/Ast/Return.h
        src/venera/Parsing/Ast/Return.cpp
        src/venera/Parsing/Ast.h
        src/venera/Parsing/Parser.h
        src/venera/Parsing/Parser.cpp
        src/venera/Analysis/Analyser.h
        src/venera/Analysis/Analyser.cpp
        src/venera/CodeGeneration/IRGenerator.h
        src/venera/CodeGeneration/IRGenerator.cpp
        src/venera/Linker.h
        src/venera/Linker.cpp
 )

target_include_directories(venera PUBLIC src)
target_include_directories(venera PRIVATE ${LLVM_INCLUDE_DIRS})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
target_compile_definitions(venera PRIVATE ${LLVM_DEFINITIONS_LIST})

if(LLVM_LINK_LLVM_DYLIB)
    set(LLVM_LIBS LLVM)
else()
    llvm_map_components_to_libnames(LLVM_LIBS core)
endif()
target_link_libraries(venera PRIVATE ${LLVM_LIBS})