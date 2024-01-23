#include "Global.h"
#include <GMLIB/Files/JsonFile.h>

namespace GMLIB::Files::JsonFile {

nlohmann::json initJson(std::string path, nlohmann::json& defaultFile) {
    writeOrUpdateFile(path, defaultFile);
    return readFromFile(path);
}

void writeOrUpdateFile(std::string path, nlohmann::json& defaultFile) {
    auto dirPath = std::filesystem::path(path).parent_path();
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
    if (!std::filesystem::exists(path)) {
        writeFile(path, defaultFile);
    } else {
        updateFile(path, defaultFile);
    }
}

nlohmann::json initJson(std::string path, std::string& defaultFile) {
    auto file = nlohmann::json::parse(defaultFile, nullptr, true, true);
    return initJson(path, file);
}

void updateFile(std::string& path, nlohmann::json& newFile) {
    auto oldFile = readFromFile(path);
    newFile.merge_patch(oldFile);
    writeFile(path, newFile);
}

void updateFile(std::string& path, std::string& newFile) {
    auto file = nlohmann::json::parse(newFile, nullptr, true, true);
    updateFile(path, file);
}

nlohmann::json readFromFile(std::string& path) {
    std::ifstream inputFile(path);
    std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    return nlohmann::json::parse(fileContent, nullptr, true, true);
}

bool writeFile(std::string& path, nlohmann::json& json) {
    try {
        std::ofstream newFile(path);
        if (!newFile.is_open()) {
            return false;
        }
        newFile << json.dump(4);
        newFile.close();
        return true;
    } catch (...) {
        return false;
    }
}

}