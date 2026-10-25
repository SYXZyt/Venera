#include "FileTracker.h"
#include <stdexcept>

void Venera::FileTracker::Clear() {
    sTracks.clear();
}

void Venera::FileTracker::AddRegister(std::shared_ptr<File> file) {
    sTracks[file] = {};
}

void Venera::FileTracker::AddWarning(std::shared_ptr<File> file)
{
    auto it = sTracks.find(file);
    if (it == sTracks.end())
        throw std::runtime_error("File not found");

    ++it->second.warningCount;
}

void Venera::FileTracker::AddError(std::shared_ptr<File> file)
{
    auto it = sTracks.find(file);
    if (it == sTracks.end())
        throw std::runtime_error("File not found");

    ++it->second.errorCount;
}

void Venera::FileTracker::SetFailed(std::shared_ptr<File> file)
{
    auto it = sTracks.find(file);
    if (it == sTracks.end())
        throw std::runtime_error("File not found");

    it->second.failed = true;
}

std::tuple<int, int> Venera::FileTracker::GetCount(std::shared_ptr<File> file)
{
    auto it = sTracks.find(file);
    if (it == sTracks.end())
        throw std::runtime_error("File not found");

    return { it->second.errorCount, it->second.warningCount };
}

std::vector<std::shared_ptr<Venera::File>> Venera::FileTracker::GetErroredOrWarnedFiles()
{
    std::vector<std::shared_ptr<File>> files;

    for (auto& [file, track] : sTracks)
    {
        if (track.errorCount > 0 || track.warningCount > 0)
            files.push_back(file);
    }

    return files;
}