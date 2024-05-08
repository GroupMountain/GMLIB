#include "Global.h"
#include <GMLIB/Files/FileUtils.h>

namespace GMLIB::Files::FileUtils {

std::vector<std::string> getAllFileNameInDirectory(std::filesystem::path const& path) {
    std::vector<std::string> fileNames;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            fileNames.push_back(entry.path().stem().string());
        }
    }
    return fileNames;
}

std::vector<std::string> getAllFileFullNameInDirectory(std::filesystem::path const& path) {
    std::vector<std::string> fileNames;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            fileNames.push_back(entry.path().filename().string());
        }
    }
    return fileNames;
}

std::vector<std::string> getAllFilePathInDirectory(std::filesystem::path const& path) {
    std::vector<std::string> fileNames;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            fileNames.push_back(entry.path().string());
        }
    }
    return fileNames;
}

} // namespace GMLIB::Files::FileUtils
