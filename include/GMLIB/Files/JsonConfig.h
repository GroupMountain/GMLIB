#pragma once
#include "GMLIB/GMLIB.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Files {

class JsonConfig {
private:
    std::string            mFilePath;
    nlohmann::ordered_json mValue;

public:
    GMLIB_API JsonConfig(std::string filePath, std::string& defaultJson);
    GMLIB_API JsonConfig(std::string filePath, nlohmann::ordered_json& defaultJson);

    JsonConfig() = delete;

public:
    virtual ~JsonConfig();

public:
    GMLIB_API bool initConfig();

    template <typename T>
    inline std::optional<T> getValue(std::vector<std::string> keyPath) {
        try {
            if (keyPath.empty()) {
                return {};
            }
            auto value = mValue;
            for (auto& key : keyPath) {
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
    inline T getValue(std::vector<std::string> keyPath, T defaultValue) {
        try {
            std::optional<T> result = getValue<T>(keyPath);
            if (result.has_value()) {
                return result.value();
            }
            this->setValue<T>(keyPath, defaultValue);
            return defaultValue;
        } catch (...) {
            this->setValue<T>(keyPath, defaultValue);
            return defaultValue;
        }
    }

    template <typename T>
    inline bool setValue(std::vector<std::string> keyPath, T data) {
        try {
            if (keyPath.empty()) {
                return false;
            }
            nlohmann::ordered_json* jsonObj = &mValue;
            for (auto& key : keyPath) {
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

    GMLIB_API bool deleteKey(std::vector<std::string> keyPath);

    GMLIB_API nlohmann::ordered_json getSelf();

    GMLIB_API bool writeFile();
};

} // namespace GMLIB::Files