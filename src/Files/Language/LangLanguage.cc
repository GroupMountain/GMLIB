#include "Global.h"
#include <GMLIB/Files/Language/LangLanguage.h>
#include <ll/api/io/FileUtils.h>

namespace GMLIB::Files {

LangLanguage::LangLanguage(std::string& filePath, std::string& defaultLanguage) : mFilePath(filePath) {
    auto data = McLang::prase(defaultLanguage);
    merge_patch(data);
}

bool LangLanguage::init() {
    auto dirPath = std::filesystem::path(mFilePath).parent_path();
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
    if (std::filesystem::exists(mFilePath)) {
        auto oldData = McLang::prase_file(mFilePath);
        merge_patch(oldData);
    }
    return write_to_file(mFilePath);
}

bool LangLanguage::reload() {
    auto file = ll::utils::file_utils::readFile(mFilePath);
    if (file.has_value()) {
        auto newData = McLang::prase(file.value());
        merge_patch(newData);
        return save_file();
    }
    return false;
}

GMLIB_API bool LangLanguage::save_file() { return write_to_file(mFilePath); }

} // namespace GMLIB::Files