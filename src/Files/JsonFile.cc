#include "Global.h"
#include <GMLIB/Files/JsonFile.h>

namespace GMLIB::Files::JsonFile {

nlohmann::ordered_json initJson(std::string path, nlohmann::ordered_json& defaultFile) {
    writeOrUpdateFile(path, defaultFile);
    return readFromFile(path);
}

void writeOrUpdateFile(std::string path, nlohmann::ordered_json& defaultFile) {
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

nlohmann::ordered_json initJson(std::string path, std::string& defaultFile) {
    auto file = nlohmann::ordered_json::parse(defaultFile, nullptr, true, true);
    return initJson(path, file);
}

void updateFile(std::string& path, nlohmann::ordered_json& newFile) {
    auto oldFile = readFromFile(path);
    newFile.merge_patch(oldFile);
    writeFile(path, newFile);
}

void updateFile(std::string& path, std::string& newFile) {
    auto file = nlohmann::ordered_json::parse(newFile, nullptr, true, true);
    updateFile(path, file);
}

nlohmann::ordered_json readFromFile(std::string& path) {
    std::ifstream inputFile(path);
    std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    return nlohmann::ordered_json::parse(fileContent, nullptr, true, true);
}

bool writeFile(std::string& path, nlohmann::ordered_json& json) {
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

bool deleteKey(nlohmann::ordered_json& json, std::vector<std::string> keyPath) {
    try {
        if (keyPath.empty()) {
            return false;
        }
        nlohmann::ordered_json* jsonObj = &json;
        for (int i = 0; i <= keyPath.size() - 2; i++) {
            if (jsonObj->is_object() && jsonObj->contains(keyPath[i])) {
                jsonObj = &(*jsonObj)[keyPath[i]];
            } else {
                return false;
            }
        }
        jsonObj->erase(keyPath.back());
        return true;
    } catch (...) {
        return false;
    }
}

bool deleteKeyAndSave(nlohmann::ordered_json& json, std::string& filePath, std::vector<std::string> keyPath) {
    auto res = deleteKey(json, keyPath);
    if (res) {
        return writeFile(filePath, json);
    }
    return false;
}

}