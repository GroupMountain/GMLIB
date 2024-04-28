#pragma once
#include "GMLIB/Macros.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Files {

class JsonLanguage {
private:
    std::string    mFilePath;
    nlohmann::json mValue;

public:
    GMLIB_API JsonLanguage(std::string const& filePath, std::string const& defaultJson);

    GMLIB_API JsonLanguage(std::string const& filePath, nlohmann::json const& defaultJson);

    JsonLanguage() = delete;

public:
    GMLIB_API virtual ~JsonLanguage();

public:
    GMLIB_API bool init();

    GMLIB_API bool reload();

    GMLIB_API bool hasValue(std::string const& key);

    GMLIB_API std::string getValue(std::string const& key);

    GMLIB_API std::string getValue(std::string const& key, std::string const& defaultValue);

    GMLIB_API bool setValue(std::string const& key, std::string const& value, std::string const& path);

    GMLIB_API bool deleteKey(std::string const& key, std::string const& path);

    GMLIB_API bool deleteKeys(std::vector<std::string> const& keys, std::string const& path);

    GMLIB_API std::string
    translate(std::string const& key, std::vector<std::string> const& param = {}, std::string const& data = "%0$s");

    GMLIB_API std::string
              get(std::string const& key, std::vector<std::string> const& param = {}, std::string const& data = "%0$s");

    GMLIB_API nlohmann::json getSelf();

    GMLIB_API bool writeFile();
};

namespace JsonLanguageFile {

GMLIB_API nlohmann::json initLanguage(std::string const& path, nlohmann::json const& defaultFile);

GMLIB_API nlohmann::json initLanguage(std::string const& path, std::string const& defaultFile);

GMLIB_API std::string getValue(nlohmann::json const& json, std::string const& key);

GMLIB_API std::string getValue(nlohmann::json const& json, std::string const& key, std::string const& defaultValue);

GMLIB_API bool
setValue(nlohmann::json& json, std::string const& key, std::string const& value, std::string const& path);

GMLIB_API bool deleteKey(nlohmann::json& json, std::string const& key, std::string const& path);

GMLIB_API bool deleteKeys(nlohmann::json& json, std::vector<std::string> const& keys, std::string const& path);

GMLIB_API std::string translate(
    nlohmann::json const&           json,
    std::string const&              key,
    std::vector<std::string> const& param = {},
    std::string const&              data  = "%0$s"
);

} // namespace JsonLanguageFile

} // namespace GMLIB::Files