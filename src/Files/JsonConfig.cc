#include "Global.h"
#include <GMLIB/Files/JsonConfig.h>

GMLIB_JsonConfig::GMLIB_JsonConfig(std::string filePath, std::string defaultJson) : mFilePath(filePath) {
    auto json_value = nlohmann::ordered_json::parse(defaultJson, nullptr, true, true);
    mValue          = json_value;
}

GMLIB_JsonConfig::GMLIB_JsonConfig(std::string filePath, nlohmann::ordered_json defaultJson)
: mFilePath(filePath),
  mValue(defaultJson) {}

GMLIB_JsonConfig::~GMLIB_JsonConfig() { mValue.clear(); }

bool GMLIB_JsonConfig::initConfig() {
    try {
        auto dirPath = std::filesystem::path(mFilePath).parent_path();
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directories(dirPath);
        }
        if (std::filesystem::exists(mFilePath)) {
            std::ifstream inputFile(mFilePath);
            std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
            auto          dataJson = nlohmann::ordered_json::parse(fileContent, nullptr, true, true);
            mValue.merge_patch(dataJson);
        }
        return this->writeFile();
    } catch (...) {
        return false;
    }
}

bool GMLIB_JsonConfig::writeFile() {
    try {
        std::ofstream newFile(mFilePath);
        if (!newFile.is_open()) {
            return false;
        }
        newFile << mValue.dump(4);
        newFile.close();
        return true;
    } catch (...) {
        return false;
    }
}

nlohmann::ordered_json GMLIB_JsonConfig::getSelf() { return mValue; }

template <typename T>
std::optional<T> GMLIB_JsonConfig::getValue(std::vector<std::string> path) {
    try {
        if (path.empty()) {
            return {};
        }
        auto value = mValue;
        for (auto& key : path) {
            if (value.is_object() && value.contains(key)) {
                value = value[key];
            } else {
                return {};
            }
        }
        return value.get<T>();
    } catch (...) {
        return {};
    }
}

template <typename T>
T GMLIB_JsonConfig::getValue(std::vector<std::string> path, T defaultValue) {
    try {
        std::optional<T> result = getValue<T>(path);
        if (result.has_value) {
            return result.value();
        }
        this->setValue<T>(path, defaultValue);
        return defaultValue;
    } catch (...) {
        return false;
    }
}

template <typename T>
bool GMLIB_JsonConfig::setValue(std::vector<std::string> path, T data) {
    try {
        if (path.empty()) {
            return false;
        }
        nlohmann::ordered_json* jsonObj = &mValue;
        for (auto& key : path) {
            if (jsonObj->is_null()) {
                *jsonObj = nlohmann::ordered_json::object();
            }
            jsonObj = &(*jsonObj)[key];
        }
        *jsonObj = data;
        return this->writeFile();
    } catch (...) {
        return false;
    }
}

bool GMLIB_JsonConfig::deleteKey(std::vector<std::string> path) {
    try {
        if (path.empty()) {
            return false;
        }
        nlohmann::ordered_json* jsonObj = &mValue;
        for (int i = 0; i <= path.size() - 2; i++) {
            if (jsonObj->is_object() && jsonObj->contains(path[i])) {
                jsonObj = &(*jsonObj)[path[i]];
            } else {
                return false;
            }
        }
        jsonObj->erase(path.back());
        return this->writeFile();
    } catch (...) {
        return false;
    }
}