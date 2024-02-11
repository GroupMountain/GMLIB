#include "Global.h"
#include <GMLIB/Files/JsonFile.h>
#include <GMLIB/Files/JsonLanguage.h>

namespace GMLIB::Files {

JsonLanguage::JsonLanguage(std::string filePath, std::string& defaultJson) : mFilePath(filePath) {
    auto json_value = nlohmann::json::parse(defaultJson, nullptr, true, true);
    mValue          = json_value;
}

JsonLanguage::JsonLanguage(std::string filePath, nlohmann::json& defaultJson)
: mValue(defaultJson),
  mFilePath(filePath) {}

JsonLanguage::~JsonLanguage() {
    writeFile();
    mValue.clear();
}

bool JsonLanguage::init() { return JsonLanguageFile::initLanguage(mFilePath, mValue); }

std::string JsonLanguage::getValue(std::string key) { return JsonLanguageFile::getValue(mValue, key); }

std::string JsonLanguage::getValue(std::string key, std::string defaultValue) {
    return JsonLanguageFile::getValue(mValue, key, defaultValue);
}

bool JsonLanguage::setValue(std::string key, std::string value, std::string path) {
    return JsonLanguageFile::setValue(mValue, key, value, mFilePath);
}

bool JsonLanguage::deleteKey(std::string key, std::string path) {
    return JsonLanguageFile::deleteKey(mValue, key, mFilePath);
}

bool JsonLanguage::deleteKeys(std::vector<std::string> keys, std::string path) {
    JsonLanguageFile::deleteKeys(mValue, keys, mFilePath);
}

std::string
JsonLanguage::translate(std::string key, std::vector<std::string> data, std::string translateKey) {
    return JsonLanguageFile::translate(mValue, key, data, translateKey);
}

nlohmann::json JsonLanguage::getSelf() { return mValue; }

bool JsonLanguage::writeFile() { return JsonFile::writeFile(mFilePath, mValue); }

namespace JsonLanguageFile {

nlohmann::json initLanguage(std::string path, nlohmann::json& defaultFile) {
    return JsonFile::initJson(path, defaultFile);
}

nlohmann::json initLanguage(std::string path, std::string& defaultFile) {
    auto file = nlohmann::json::parse(defaultFile, nullptr, true, true);
    return initLanguage(path, file);
}

std::string getValue(nlohmann::json& json, std::string key) {
    try {
        return json[key].get<std::string>();
    } catch (...) {
        return key;
    }
}

std::string getValue(nlohmann::json& json, std::string key, std::string defaultValue) {
    try {
        return json[key].get<std::string>();
    } catch (...) {
        return defaultValue;
    }
}

bool setValue(nlohmann::json& json, std::string key, std::string value, std::string path) {
    json[key] = value;
    return JsonFile::writeFile(path, json);
}

bool deleteKey(nlohmann::json& json, std::string key, std::string path) {
    json.erase(key);
    return JsonFile::writeFile(path, json);
}

bool deleteKeys(nlohmann::json& json, std::vector<std::string> keys, std::string path) {
    for (auto& key : keys) {
        json.erase(key);
    }
    return JsonFile::writeFile(path, json);
}

std::string translate(nlohmann::json& json, std::string key, std::vector<std::string> data, std::string translateKey) {
    auto lang = getValue(json, key);
    if (data.empty()) {
        return lang;
    }
    for (int i = 0; i <= data.size() - 1; i++) {
        auto oldValue = translateKey;
        ll::utils::string_utils::replaceAll(oldValue, "0", std::to_string(i + 1));
        ll::utils::string_utils::replaceAll(lang, oldValue, data[i]);
    }
    return lang;
}

} // namespace JsonLanguageFile

} // namespace GMLIB::Files