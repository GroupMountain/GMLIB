#pragma once
#include "GMLIB/GMLIB.h"

class GMLIB_JsonConfig {
public:
    std::string            mFilePath;
    nlohmann::ordered_json mValue;

public:
    GMLIB_JsonConfig(std::string filePath, std::string defaultJson);
    GMLIB_JsonConfig(std::string filePath, nlohmann::ordered_json defaultJson);
    GMLIB_JsonConfig() = delete;

public:
    virtual ~GMLIB_JsonConfig();

public:
    GMLIB_API bool initConfig();

    template <typename T>
    GMLIB_API std::optional<T> getValue(std::vector<std::string> key);

    template <typename T>
    GMLIB_API T getValue(std::vector<std::string> key, T defaultValue);

    template <typename T>
    GMLIB_API bool setValue(std::vector<std::string> key, T value);

    GMLIB_API bool deleteKey(std::vector<std::string> key);

    GMLIB_API nlohmann::ordered_json getSelf();

    GMLIB_API bool writeFile();
};