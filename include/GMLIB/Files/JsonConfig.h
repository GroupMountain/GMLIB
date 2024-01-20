#pragma once
#include "GMLIB/GMLIB.h"
#include "nlohmann/json.hpp"

class GMLIB_JsonConfig {
public:
    std::string            mFilePath;
    nlohmann::ordered_json mValue;

public:
    GMLIB_API GMLIB_JsonConfig(std::string filePath, std::string defaultJson);
    GMLIB_API GMLIB_JsonConfig(std::string filePath, nlohmann::ordered_json defaultJson);
    GMLIB_API GMLIB_JsonConfig() = delete;

public:
    virtual ~GMLIB_JsonConfig();

public:
    GMLIB_API bool initConfig();

    template <typename T>
    inline std::optional<T> getValue(std::vector<std::string> path) {
        try {
            if (path.empty()) {
                return {};
            }
            auto value = mValue;
            for (auto& key : path) {
                if (value.is_object() && value.contains(key)) {
                    value = value[key];
                } else {
                    return {};
                }
            }
            return value.get<T>();
        } catch (...) {
            return {};
        }
    }

    template <typename T>
    inline T getValue(std::vector<std::string> path, T defaultValue) {
        try {
            std::optional<T> result = getValue<T>(path);
            if (result.has_value()) {
                return result.value();
            }
            this->setValue<T>(path, defaultValue);
            return defaultValue;
        } catch (...) {
            this->setValue<T>(path, defaultValue);
            return defaultValue;
        }
    }

    template <typename T>
    inline bool setValue(std::vector<std::string> path, T data) {
        try {
            if (path.empty()) {
                return false;
            }
            nlohmann::ordered_json* jsonObj = &mValue;
            for (auto& key : path) {
                if (jsonObj->is_null()) {
                    *jsonObj = nlohmann::ordered_json::object();
                }
                jsonObj = &(*jsonObj)[key];
            }
            *jsonObj = data;
            return this->writeFile();
        } catch (...) {
            return false;
        }
    }

    GMLIB_API bool deleteKey(std::vector<std::string> key);

    GMLIB_API nlohmann::ordered_json getSelf();

    GMLIB_API bool writeFile();
};