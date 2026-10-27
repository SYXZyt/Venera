#include "Linker.h"
#include <venera/Error.h>
#include <llvm/Support/Program.h>
#include <format>

bool Venera::Linker::Link(const std::string& objectPath, const std::string& outputPath)
{
    llvm::ErrorOr<std::string> clang = llvm::sys::findProgramByName("clang");
    if (!clang)
    {
        Error::Report("Could not find 'clang' on PATH, which is required for linking");
        return false;
    }

    std::vector<llvm::StringRef> args = { *clang, objectPath, "-o", outputPath };

    std::string errorMessage;
    int exitCode = llvm::sys::ExecuteAndWait(*clang, args, std::nullopt, {}, 0, 0, &errorMessage);

    if (exitCode != 0)
    {
        Error::Report(errorMessage.empty() ? std::format("Linking failed (clang exited with code {})", exitCode) : std::format("Linking failed: {}", errorMessage));
        return false;
    }

    return true;
}