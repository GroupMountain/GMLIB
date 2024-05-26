#include "Global.h"
#include <GMLIB/Files/JsonFile.h>
#include <GMLIB/Files/Language/JsonLanguage.h>

namespace GMLIB::Files {

JsonLanguage::JsonLanguage(std::filesystem::path const& filePath, std::string const& defaultJson)
: mFilePath(filePath) {
    auto data = McLang::from_json(defaultJson);
    merge_patch(data);
}

JsonLanguage::JsonLanguage(std::filesystem::path const& filePath, nlohmann::json const& defaultJson)
: mFilePath(filePath) {
    auto data = McLang::from_json(defaultJson);
    merge_patch(data);
}

JsonLanguage::JsonLanguage(
    std::filesystem::path const&                        filePath,
    std::unordered_map<std::string, std::string> const& defaultLanguage
)
: mFilePath(filePath) {
    auto data = McLang(defaultLanguage);
    merge_patch(data);
}

JsonLanguage::JsonLanguage(std::filesystem::path const& filePath, McLang const& defaultLanguage) : mFilePath(filePath) {
    merge_patch(defaultLanguage);
}

JsonLanguage::~JsonLanguage() { save_file(); }

bool JsonLanguage::init() {
    auto dirPath = std::filesystem::path(mFilePath).parent_path();
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
    if (std::filesystem::exists(mFilePath)) {
        auto oldFile = JsonFile::readFromFile(mFilePath);
        auto oldData = McLang::from_json(oldFile);
        merge_patch(oldData);
    }
    return save_file();
}

bool JsonLanguage::reload() {
    if (std::filesystem::exists(mFilePath)) {
        auto newFile = JsonFile::readFromFile(mFilePath);
        auto newData = McLang::from_json(newFile);
        merge_patch(newData);
        return save_file();
    }
    return false;
}

bool JsonLanguage::save_file() { return JsonFile::writeFile(mFilePath, to_json()); }

} // namespace GMLIB::Files