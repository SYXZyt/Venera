#include "Helpers.h"
#include <sstream>

std::string Venera::CharToString(char c) {
    return std::string(1, c);
}

std::string Venera::RemoveEscape(const std::string& s)
{
    std::stringstream ss;

    for (char c : s)
        ss << RemoveEscape(c);

    return ss.str();
}

std::string Venera::RemoveEscape(char c)
{
    switch (c)
    {
        case '\n': return "\\n";
        case '\t': return "\\t";
        case '\0': return "\\0";
        case '\a': return "\\a";
        case '\b': return "\\b";
        case '\r': return "\\r";
        default: return CharToString(c);
    }
}

std::string Venera::Replace(const std::string& string, std::string_view from, std::string_view to)
{
    if (from.empty())
        return string;

    std::stringstream ss;
    size_t position = 0;

    while (true)
    {
        size_t found = string.find(from, position);

        if (found == std::string::npos)
        {
            ss << string.substr(position);
            break;
        }

        ss << string.substr(position, found - position);
        ss << to;

        position = found + from.size();
    }

    return ss.str();
}

std::vector<std::string> Venera::Split(const std::string& string, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(string);
    std::string part;

    while (std::getline(ss, part, delim))
        result.push_back(part);

    return result;
}

std::string Venera::Join(const std::string& string, std::span<std::string> strings)
{
    std::stringstream ss;

    for (size_t i = 0; i < strings.size(); ++i)
    {
        if (i > 0)
            ss << string;

        ss << strings[i];
    }

    return ss.str();
}

std::string Venera::Substring(const std::string& string, size_t offset) {
    return string.substr(offset, string.size() - offset);
}

std::string Venera::SubstringFromEnd(const std::string& string, size_t offset) {
    return string.substr(string.size() - offset, offset);
}