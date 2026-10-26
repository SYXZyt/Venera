#pragma once
#include <string>
#include <vector>
#include <span>
#include <type_traits>
#include <charconv>
#include <algorithm>
#include <string_view>

namespace Venera
{
    extern std::string CharToString(char c);
    extern std::string RemoveEscape(const std::string& s);
    extern std::string RemoveEscape(char c);

    extern std::string Replace(const std::string& string, std::string_view from, std::string_view to);
    extern std::vector<std::string> Split(const std::string& string, char delim);
    extern std::string Join(const std::string& string, std::span<std::string> strings);

    extern std::string Substring(const std::string& string, size_t offset);
    extern std::string SubstringFromEnd(const std::string& string, size_t offset);

    template <typename T>
    inline bool TryParseInt(std::string_view string, T& result)
    {
        auto [ptr, error] = std::from_chars(string.data(), string.data() + string.size(), result);
        return error == std::errc() && ptr == string.data() + string.size();
    }

    template <typename T>
    inline void AddRange(std::vector<T>& a, const std::vector<T>& b)
    {
        a.reserve(a.size() + b.size());
        a.insert(a.end(), b.begin(), b.end());
    }

    template <typename T, typename... Args>
    constexpr bool IsOneOf(const T& value, const Args&... args) {
        return ((value == args) || ...);
    }
}