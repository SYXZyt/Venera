#pragma once
#include <memory>
#include <vector>
#include <string>
#include <string_view>

namespace Venera
{
    class File final
    {
    public:
        static std::shared_ptr<File> GetEmpty() {
            return FromSource("", "");
        }

        static std::shared_ptr<File> FromSource(std::string_view name, std::string_view contents) {
            return std::make_shared<File>(name, contents);
        }

    private:
        std::vector<std::string> mLines;
        std::string mContents;
        std::string mName;

    public:
        const std::vector<std::string>& Lines() const {
            return mLines;
        }

        std::string_view Name() const {
            return mName;
        }

        std::string_view Contents() const {
            return mContents;
        }

        File(std::string_view name, std::string_view contents);
    };
}