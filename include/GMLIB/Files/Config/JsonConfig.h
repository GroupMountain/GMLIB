#pragma once
#include "GMLIB/Macros.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Files {

class JsonConfig {
private:
    std::string            mFilePath;
    nlohmann::ordered_json mValue;

public:
    [[deprecated]] GMLIB_API JsonConfig(std::string const& filePath, std::string const& defaultJson);
    [[deprecated]] GMLIB_API JsonConfig(std::string const& filePath, nlohmann::ordered_json const& defaultJson);

    JsonConfig() = delete;

public:
    [[deprecated]] GMLIB_API virtual ~JsonConfig();

public:
    [[deprecated]] GMLIB_API bool init();

    [[deprecated]] GMLIB_API bool reload();

    template <typename T>
    [[deprecated]] inline std::optional<T> getValue(std::vector<std::string> const& keyPath) {
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
    [[deprecated]] inline T getValue(std::vector<std::string> const& keyPath, T defaultValue) {
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
    [[deprecated]] inline bool setValue(std::vector<std::string> const& keyPath, T data) {
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

    [[deprecated]] GMLIB_API bool deleteKey(std::vector<std::string> const& keyPath);

    [[deprecated]] GMLIB_API nlohmann::ordered_json getSelf();

    [[deprecated]] GMLIB_API bool writeFile();
};

} // namespace GMLIB::Files