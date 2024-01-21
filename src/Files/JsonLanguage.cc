#include "Global.h"
#include <GMLIB/Files/JsonLanguage.h>

namespace GMLIB::Files::JsonLanguage {

void updateFile(std::string path, nlohmann::json newFile) {
    auto oldFile = readFromFile(path);
    newFile.merge_patch(oldFile);
    writeFile(path, newFile);
}

void updateFile(std::string path, std::string newFile) {
    auto file = nlohmann::json::parse(newFile, nullptr, true, true);
    updateFile(path, file);
}

nlohmann::json readFromFile(std::string path) {
    std::ifstream inputFile(path);
    std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    return nlohmann::json::parse(fileContent, nullptr, true, true);
}

bool writeFile(std::string path, nlohmann::json json) {
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

std::string getValue(nlohmann::json json, std::string key) {
    try {
        return json[key].get<std::string>();
    } catch (...) {
        return key;
    }
}

std::string getValue(nlohmann::json json, std::string key, std::string defaultValue) {
    try {
        return json[key].get<std::string>();
    } catch (...) {
        return defaultValue;
    }
}

bool setValue(nlohmann::json json, std::string key, std::string value, std::string path) {
    json[key] = value;
    return writeFile(path, json);
}

bool deleteKey(nlohmann::json json, std::string key, std::string path) {
    json.erase(key);
    return writeFile(path, json);
}

bool deleteKeys(nlohmann::json json, std::vector<std::string> keys, std::string path) {
    for (auto& key : keys) {
        json.erase(key);
    }
    return writeFile(path, json);
}

std::string translate(nlohmann::json json, std::string key, std::vector<std::string> data, std::string translateKey) {
    auto lang = getValue(json, key);
    for (int i = 0; i <= data.size() - 1; i++) {
        auto oldValue = translateKey;
        ll::utils::string_utils::replaceAll(oldValue, "0", std::to_string(i + 1));
        ll::utils::string_utils::replaceAll(lang, oldValue, data[i]);
    }
    return lang;
}

} // namespace GMLIB::JsonLanguage