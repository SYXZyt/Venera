#pragma once
#include <string>

namespace Venera
{
    class Linker final
    {
    private:
        Linker() = delete;

    public:
        static bool Link(const std::string& objectPath, const std::string& outputPath);
    };
}