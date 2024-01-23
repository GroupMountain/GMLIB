#include "Global.h"
#include <GMLIB/Files/JsonFile.h>
#include <GMLIB/Files/JsonLanguage.h>

namespace GMLIB::Files::JsonLanguage {

nlohmann::json initLanguage(std::string path, nlohmann::json& defaultFile) {
    return GMLIB::Files::JsonFile::initJson(path, defaultFile);
}

nlohmann::json initLanguage(std::string path, std::string& defaultFile) {
    return GMLIB::Files::JsonFile::initJson(path, defaultFile);
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

} // namespace GMLIB::Files::JsonLanguage