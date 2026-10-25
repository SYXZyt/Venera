#pragma once
#include <string>
#include <luna/Type.h>

namespace Luna::ANSI
{
    extern bool SupportsANSI();
    extern const char* const Reset();

    namespace Colours
    {
        extern std::string MakeIndexed(uint8 idx);

        static inline const std::string Purple = MakeIndexed(5);
        static inline const std::string Red = MakeIndexed(9);
        static inline const std::string Yellow = MakeIndexed(11);
        static inline const std::string Aqua = MakeIndexed(14);
        static inline const std::string Grey = MakeIndexed(248);
        static inline const std::string Green = MakeIndexed(2);
    }
}