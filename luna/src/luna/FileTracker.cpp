#include "FileTracker.h"
#include <stdexcept>

void Luna::FileTracker::Clear() {
    sTracks.clear();
}

void Luna::FileTracker::AddRegister(std::shared_ptr<File> file) {
    sTracks[file] = {};
}

void Luna::FileTracker::AddWarning(std::shared_ptr<File> file)
{
    auto it = sTracks.find(file);
    if (it == sTracks.end())
        throw std::runtime_error("File not found");

    ++it->second.warningCount;
}

void Luna::FileTracker::AddError(std::shared_ptr<File> file)
{
    auto it = sTracks.find(file);
    if (it == sTracks.end())
        throw std::runtime_error("File not found");

    ++it->second.errorCount;
}

void Luna::FileTracker::SetFailed(std::shared_ptr<File> file)
{
    auto it = sTracks.find(file);
    if (it == sTracks.end())
        throw std::runtime_error("File not found");

    it->second.failed = true;
}

std::tuple<int, int> Luna::FileTracker::GetCount(std::shared_ptr<File> file)
{
    auto it = sTracks.find(file);
    if (it == sTracks.end())
        throw std::runtime_error("File not found");

    return { it->second.errorCount, it->second.warningCount };
}

std::vector<std::shared_ptr<Luna::File>> Luna::FileTracker::GetErroredOrWarnedFiles()
{
    std::vector<std::shared_ptr<File>> files;

    for (auto& [file, track] : sTracks)
    {
        if (track.errorCount > 0 || track.warningCount > 0)
            files.push_back(file);
    }

    return files;
}