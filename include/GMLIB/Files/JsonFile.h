#pragma once
#include "GMLIB/GMLIB.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Files::JsonFile {

GMLIB_API nlohmann::json initJson(std::string path, nlohmann::json& defaultFile);

GMLIB_API nlohmann::json initJson(std::string path, std::string& defaultFile);

GMLIB_API void writeOrUpdateFile(std::string path, nlohmann::json& defaultFile);

GMLIB_API void updateFile(std::string& path, nlohmann::json& newFile);

GMLIB_API void updateFile(std::string& path, std::string& newFile);

GMLIB_API nlohmann::json readFromFile(std::string& path);

GMLIB_API bool writeFile(std::string& path, nlohmann::json& json);

} // namespace GMLIB::Files::Json