#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Files {

class JsonLanguage {
private:
    std::string    mFilePath;
    nlohmann::json mValue;

public:
    GMLIB_API JsonLanguage(std::string filePath, std::string& defaultJson);

    GMLIB_API JsonLanguage(std::string filePath, nlohmann::json& defaultJson);

    JsonLanguage() = delete;

public:
    GMLIB_API virtual ~JsonLanguage();

public:
    GMLIB_API bool init();

    GMLIB_API bool hasValue(std::string key);

    GMLIB_API std::string getValue(std::string key);

    GMLIB_API std::string getValue(std::string key, std::string defaultValue);

    GMLIB_API bool setValue(std::string key, std::string value, std::string path);

    GMLIB_API bool deleteKey(std::string key, std::string path);

    GMLIB_API bool deleteKeys(std::vector<std::string> keys, std::string path);

    GMLIB_API std::string
              translate(std::string key, std::vector<std::string> data = {}, std::string translateKey = "%0$s");

    GMLIB_API nlohmann::json getSelf();

    GMLIB_API bool writeFile();
};

namespace JsonLanguageFile {

GMLIB_API nlohmann::json initLanguage(std::string path, nlohmann::json& defaultFile);

GMLIB_API nlohmann::json initLanguage(std::string path, std::string& defaultFile);

GMLIB_API std::string getValue(nlohmann::json& json, std::string key);

GMLIB_API std::string getValue(nlohmann::json& json, std::string key, std::string defaultValue);

GMLIB_API bool setValue(nlohmann::json& json, std::string key, std::string value, std::string path);

GMLIB_API bool deleteKey(nlohmann::json& json, std::string key, std::string path);

GMLIB_API bool deleteKeys(nlohmann::json& json, std::vector<std::string> keys, std::string path);

GMLIB_API std::string
translate(nlohmann::json& json, std::string key, std::vector<std::string> data = {}, std::string translateKey = "%0$s");

} // namespace JsonLanguageFile

} // namespace GMLIB::Files