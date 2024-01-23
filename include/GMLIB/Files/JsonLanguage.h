#pragma once
#include "GMLIB/Files/JsonConfig.h"
#include "GMLIB/GMLIB.h"

namespace GMLIB::Files::JsonLanguage {

GMLIB_API nlohmann::json initLanguage(std::string path, nlohmann::json& defaultFile);

GMLIB_API nlohmann::json initLanguage(std::string path, std::string& defaultFile);

GMLIB_API std::string getValue(nlohmann::json& json, std::string key);

GMLIB_API std::string getValue(nlohmann::json& json, std::string key, std::string defaultValue);

GMLIB_API bool setValue(nlohmann::json& json, std::string key, std::string value, std::string path);

GMLIB_API bool deleteKey(nlohmann::json& json, std::string key, std::string path);

GMLIB_API bool deleteKeys(nlohmann::json& json, std::vector<std::string> keys, std::string path);

GMLIB_API std::string
translate(nlohmann::json& json, std::string key, std::vector<std::string> data = {}, std::string translateKey = "%0$s");

} // namespace GMLIB::Files::JsonLanguage