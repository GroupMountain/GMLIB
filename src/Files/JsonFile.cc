#include "Global.h"
#include <GMLIB/Files/JsonFile.h>

namespace GMLIB::Files::JsonFile {

nlohmann::ordered_json initOrderedJson(std::string const& path, nlohmann::ordered_json const& defaultFile) {
    writeOrUpdateOrderedFile(path, defaultFile);
    return readFromFile(path);
}

nlohmann::json initJson(std::string const& path, nlohmann::json const& defaultFile) {
    writeOrUpdateFile(path, defaultFile);
    return readFromFile(path);
}

void writeOrUpdateOrderedFile(std::string const& path, nlohmann::ordered_json const& defaultFile) {
    auto dirPath = std::filesystem::path(path).parent_path();
    if (!std::filesystem::exists(dirPath)) {
        std::filesystem::create_directories(dirPath);
    }
    if (!std::filesystem::exists(path)) {
        writeOrderedFile(path, defaultFile);
    } else {
        updateOrderedFile(path, defaultFile);
    }
}

void writeOrUpdateFile(std::string const& path, nlohmann::json const& defaultFile) {
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

nlohmann::ordered_json initOrderedJson(std::string const& path, std::string const& defaultFile) {
    auto file = nlohmann::ordered_json::parse(defaultFile, nullptr, true, true);
    return initOrderedJson(path, file);
}

nlohmann::json initJson(std::string const& path, std::string const& defaultFile) {
    auto file = nlohmann::json::parse(defaultFile, nullptr, true, true);
    return initJson(path, file);
}


void updateOrderedFile(std::string const& path, nlohmann::ordered_json const& file) {
    auto newFile = file;
    try {
        auto oldFile = readFromFile(path);
        newFile.merge_patch(oldFile);
    } catch (...) {
        auto backupPath = path + "_old";
        std::filesystem::rename(path, backupPath);
    }
    writeOrderedFile(path, newFile);
}

void updateFile(std::string const& path, nlohmann::json const& file) {
    auto newFile = file;
    try {
        auto oldFile = readFromFile(path);
        newFile.merge_patch(oldFile);
    } catch (...) {
        auto backupPath = path + "_old";
        std::filesystem::rename(path, backupPath);
    }
    writeFile(path, newFile);
}

void updateOrderedFile(std::string const& path, std::string const& newFile) {
    auto file = nlohmann::ordered_json::parse(newFile, nullptr, true, true);
    updateOrderedFile(path, file);
}

void updateFile(std::string const& path, std::string const& newFile) {
    auto file = nlohmann::json::parse(newFile, nullptr, true, true);
    updateFile(path, file);
}

nlohmann::ordered_json readFromOrderedFile(std::string const& path) {
    std::ifstream inputFile(path);
    std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();
    return nlohmann::ordered_json::parse(fileContent, nullptr, true, true);
}

nlohmann::json readFromFile(std::string const& path) {
    std::ifstream inputFile(path);
    std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();
    return nlohmann::json::parse(fileContent, nullptr, true, true);
}

bool writeOrderedFile(std::string const& path, nlohmann::ordered_json const& json) {
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

bool writeFile(std::string const& path, nlohmann::json const& json) {
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

bool deleteOrderedKey(nlohmann::ordered_json& json, std::vector<std::string> const& keyPath) {
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

bool deleteKey(nlohmann::json& json, std::vector<std::string> const& keyPath) {
    try {
        if (keyPath.empty()) {
            return false;
        }
        nlohmann::json* jsonObj = &json;
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

bool deleteOrderedKeyAndSave(
    nlohmann::ordered_json&         json,
    std::string const&              filePath,
    std::vector<std::string> const& keyPath
) {
    auto res = deleteOrderedKey(json, keyPath);
    if (res) {
        return writeOrderedFile(filePath, json);
    }
    return false;
}

bool deleteKeyAndSave(nlohmann::json& json, std::string const& filePath, std::vector<std::string> const& keyPath) {
    auto res = deleteKey(json, keyPath);
    if (res) {
        return writeFile(filePath, json);
    }
    return false;
}

} // namespace GMLIB::Files::JsonFile