#include "Global.h"
#include <GMLIB/Files/JsonLanguage.h>

GMLIB_JsonLanguage::GMLIB_JsonLanguage(std::string filePath, std::string defaultJson)
: GMLIB_JsonConfig(filePath, defaultJson) {}


GMLIB_JsonLanguage::GMLIB_JsonLanguage(std::string filePath, nlohmann::ordered_json defaultJson)
: GMLIB_JsonConfig(filePath, defaultJson) {}

std::string GMLIB_JsonLanguage::getValue(std::string key) {
    try {
        return mValue[key].get<std::string>();
    } catch (...) {
        return key;
    }
}

std::string GMLIB_JsonLanguage::getValue(std::string key, std::string defaultValue) {
    try {
        return mValue[key].get<std::string>();
    } catch (...) {
        mValue[key] = defaultValue;
        writeFile();
        return defaultValue;
    }
}

bool GMLIB_JsonLanguage::setValue(std::string key, std::string value) {
    mValue[key] = value;
    return writeFile();
}

bool GMLIB_JsonLanguage::deleteKey(std::string key) {
    mValue.erase(key);
    return writeFile();
}

std::string GMLIB_JsonLanguage::translate(std::string key, std::vector<std::string> data, std::string translateKey) {
    auto lang = getValue(key);
    for (int i = 0; i <= data.size() - 1; i++) {
        auto oldValue = translateKey;
        ll::utils::string_utils::replaceAll(oldValue, "0", std::to_string(i + 1));
        ll::utils::string_utils::replaceAll(lang, oldValue, data[i]);
    }
    return lang;
}