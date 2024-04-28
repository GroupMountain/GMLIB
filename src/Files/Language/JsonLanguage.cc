#include "Global.h"
#include <GMLIB/Files/JsonFile.h>
#include <GMLIB/Files/Language/JsonLanguage.h>

namespace GMLIB::Files {

JsonLanguage::JsonLanguage(std::string const& filePath, std::string const& defaultJson) : mFilePath(filePath) {
    auto json_value = nlohmann::json::parse(defaultJson, nullptr, true, true);
    mValue          = json_value;
}

JsonLanguage::JsonLanguage(std::string const& filePath, nlohmann::json const& defaultJson)
: mValue(defaultJson),
  mFilePath(filePath) {}

JsonLanguage::~JsonLanguage() { writeFile(); }

bool JsonLanguage::init() {
    if (!mFilePath.empty()) {
        mValue = JsonLanguageFile::initLanguage(mFilePath, mValue);
        return true;
    }
    return false;
}

bool JsonLanguage::reload() {
    auto newFile = JsonFile::readFromFile(mFilePath);
    mValue.merge_patch(newFile);
    return this->writeFile();
}

bool JsonLanguage::hasValue(std::string const& key) { return mValue.contains(key); }

std::string JsonLanguage::getValue(std::string const& key) { return JsonLanguageFile::getValue(mValue, key); }

std::string JsonLanguage::getValue(std::string const& key, std::string const& defaultValue) {
    return JsonLanguageFile::getValue(mValue, key, defaultValue);
}

bool JsonLanguage::setValue(std::string const& key, std::string const& value, std::string const& path) {
    return JsonLanguageFile::setValue(mValue, key, value, mFilePath);
}

bool JsonLanguage::deleteKey(std::string const& key, std::string const& path) {
    return JsonLanguageFile::deleteKey(mValue, key, mFilePath);
}

bool JsonLanguage::deleteKeys(std::vector<std::string> const& keys, std::string const& path) {
    return JsonLanguageFile::deleteKeys(mValue, keys, mFilePath);
}

std::string
JsonLanguage::translate(std::string const& key, std::vector<std::string> const& data, std::string const& translateKey) {
    return JsonLanguageFile::translate(mValue, key, data, translateKey);
}

std::string
JsonLanguage::get(std::string const& key, std::vector<std::string> const& data, std::string const& translateKey) {
    return JsonLanguageFile::translate(mValue, key, data, translateKey);
}

nlohmann::json JsonLanguage::getSelf() { return mValue; }

bool JsonLanguage::writeFile() { return JsonFile::writeFile(mFilePath, mValue); }

namespace JsonLanguageFile {

nlohmann::json initLanguage(std::string const& path, nlohmann::json const& defaultFile) {
    return JsonFile::initJson(path, defaultFile);
}

nlohmann::json initLanguage(std::string const& path, std::string const& defaultFile) {
    auto file = nlohmann::json::parse(defaultFile, nullptr, true, true);
    return initLanguage(path, file);
}

std::string getValue(nlohmann::json const& json, std::string const& key) {
    try {
        return json[key].get<std::string>();
    } catch (...) {
        return key;
    }
}

std::string getValue(nlohmann::json const& json, std::string const& key, std::string const& defaultValue) {
    try {
        return json[key].get<std::string>();
    } catch (...) {
        return defaultValue;
    }
}

bool setValue(nlohmann::json& json, std::string const& key, std::string const& value, std::string const& path) {
    json[key] = value;
    return JsonFile::writeFile(path, json);
}

bool deleteKey(nlohmann::json& json, std::string const& key, std::string const& path) {
    json.erase(key);
    return JsonFile::writeFile(path, json);
}

bool deleteKeys(nlohmann::json& json, std::vector<std::string> const& keys, std::string const& path) {
    for (auto& key : keys) {
        json.erase(key);
    }
    return JsonFile::writeFile(path, json);
}

std::string translate(
    nlohmann::json const&           json,
    std::string const&              key,
    std::vector<std::string> const& data,
    std::string const&              translateKey
) {
    auto lang = getValue(json, key);
    if (data.empty()) {
        return lang;
    }
    if (translateKey == "%0$s" && data.size() == 1) {
        ll::utils::string_utils::replaceAll(lang, "%s", data[0]);
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