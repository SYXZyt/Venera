#pragma once
#include <tuple>
#include <unordered_map>
#include <vector>
#include <venera/File.h>

namespace Venera
{
    class FileTracker final
    {
    private:
        FileTracker() = delete;

    public:
        struct Track final
        {
            int errorCount = 0;
            int warningCount = 0;
            bool failed = false;
        };

    private:
        static inline std::unordered_map<std::shared_ptr<File>, Track> sTracks = {};

    public:
        static void Clear();

        static void AddRegister(std::shared_ptr<File> file);

        static void AddWarning(std::shared_ptr<File> file);
        static void AddError(std::shared_ptr<File> file);

        static void SetFailed(std::shared_ptr<File> file);

        static std::tuple<int, int> GetCount(std::shared_ptr<File> file);

        static std::vector<std::shared_ptr<File>> GetErroredOrWarnedFiles();
    };
}