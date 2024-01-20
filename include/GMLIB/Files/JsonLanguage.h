#pragma once
#include "GMLIB/GMLIB.h"
#include "GMLIB/Files/JsonConfig.h"

class GMLIB_JsonLanguage : public GMLIB_JsonConfig {
public:
    
public:
    GMLIB_API GMLIB_JsonLanguage(std::string filePath, std::string defaultJson);
    GMLIB_API GMLIB_JsonLanguage(std::string filePath, nlohmann::ordered_json defaultJson);
    GMLIB_API GMLIB_JsonLanguage() = delete;

public:
    GMLIB_API std::string getValue(std::string key);

    GMLIB_API std::string getValue(std::string key, std::string defaultValue);

    GMLIB_API bool setValue(std::string key, std::string value);

    GMLIB_API bool deleteKey(std::string key);

    GMLIB_API std::string translate(std::string key, std::vector<std::string> data = {}, std::string translateKey = "%0$s");
};