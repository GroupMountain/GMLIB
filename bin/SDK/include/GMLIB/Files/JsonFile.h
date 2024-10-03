#pragma once
#include "GMLIB/Macros.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Files::JsonFile {

GMLIB_API nlohmann::ordered_json
          initOrderedJson(std::filesystem::path const& path, nlohmann::ordered_json const& defaultFile);

GMLIB_API nlohmann::ordered_json initOrderedJson(std::filesystem::path const& path, std::string const& defaultFile);

GMLIB_API nlohmann::json initJson(std::filesystem::path const& path, nlohmann::json const& defaultFile);

GMLIB_API nlohmann::json initJson(std::filesystem::path const& path, std::string const& defaultFile);

GMLIB_API void writeOrUpdateOrderedFile(std::filesystem::path const& path, nlohmann::ordered_json const& defaultFile);

GMLIB_API void writeOrUpdateFile(std::filesystem::path const& path, nlohmann::json const& defaultFile);

GMLIB_API void updateOrderedFile(std::filesystem::path const& path, nlohmann::ordered_json const& newFile);

GMLIB_API void updateOrderedFile(std::filesystem::path const& path, std::string const& newFile);

GMLIB_API void updateFile(std::filesystem::path const& path, nlohmann::json const& newFile);

GMLIB_API void updateFile(std::filesystem::path const& path, std::string const& newFile);

GMLIB_API nlohmann::ordered_json readFromOrderedFile(std::filesystem::path const& path);

GMLIB_API nlohmann::json readFromFile(std::filesystem::path const& path);

GMLIB_API bool writeOrderedFile(std::filesystem::path const& path, nlohmann::ordered_json const& json);

GMLIB_API bool writeFile(std::filesystem::path const& path, nlohmann::json const& json);

template <typename T>
inline std::optional<T> getValue(nlohmann::ordered_json& json, std::vector<std::string> const& keyPath) {
    try {
        if (keyPath.empty()) {
            return {};
        }
        auto value = json;
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
inline std::optional<T> getValue(nlohmann::json& json, std::vector<std::string> const& keyPath) {
    try {
        if (keyPath.empty()) {
            return {};
        }
        auto value = json;
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
inline T getValue(nlohmann::ordered_json& json, std::vector<std::string> const& keyPath, T defaultValue) {
    try {
        std::optional<T> result = getValue<T>(json, keyPath);
        if (result.has_value()) {
            return result.value();
        }
        setValue<T>(json, keyPath, defaultValue);
        return defaultValue;
    } catch (...) {
        setValue<T>(json, keyPath, defaultValue);
        return defaultValue;
    }
}

template <typename T>
inline T getValue(nlohmann::json& json, std::vector<std::string> const& keyPath, T defaultValue) {
    try {
        std::optional<T> result = getValue<T>(json, keyPath);
        if (result.has_value()) {
            return result.value();
        }
        setValue<T>(json, keyPath, defaultValue);
        return defaultValue;
    } catch (...) {
        setValue<T>(json, keyPath, defaultValue);
        return defaultValue;
    }
}

template <typename T>
inline bool setValue(nlohmann::ordered_json& json, std::vector<std::string> const& keyPath, T data) {
    try {
        if (keyPath.empty()) {
            return false;
        }
        nlohmann::ordered_json* jsonObj = &json;
        for (auto& key : keyPath) {
            if (jsonObj->is_null()) {
                *jsonObj = nlohmann::ordered_json::object();
            }
            jsonObj = &(*jsonObj)[key];
        }
        *jsonObj = data;
        return true;
    } catch (...) {
        return false;
    }
}

template <typename T>
inline bool setValue(nlohmann::json& json, std::vector<std::string> const& keyPath, T data) {
    try {
        if (keyPath.empty()) {
            return false;
        }
        nlohmann::json* jsonObj = &json;
        for (auto& key : keyPath) {
            if (jsonObj->is_null()) {
                *jsonObj = nlohmann::json::object();
            }
            jsonObj = &(*jsonObj)[key];
        }
        *jsonObj = data;
        return true;
    } catch (...) {
        return false;
    }
}

template <typename T>
inline bool setValueAndSave(
    nlohmann::ordered_json&         json,
    std::string const&              filePath,
    std::vector<std::string> const& keyPath,
    T                               data
) {
    auto res = setValue<T>(json, keyPath, data);
    if (res) {
        return writeOrderedFile(filePath, json);
    }
    return false;
}

template <typename T>
inline bool
setValueAndSave(nlohmann::json& json, std::string const& filePath, std::vector<std::string> const& keyPath, T data) {
    auto res = setValue<T>(json, keyPath, data);
    if (res) {
        return writeFile(filePath, json);
    }
    return false;
}

GMLIB_API bool deleteOrderedKey(nlohmann::ordered_json& json, std::vector<std::string> const& keyPath);

GMLIB_API bool deleteKey(nlohmann::json& json, std::vector<std::string> const& keyPath);

GMLIB_API bool deleteOrderedKeyAndSave(
    nlohmann::ordered_json&         json,
    std::string const&              filePath,
    std::vector<std::string> const& keyPath
);

GMLIB_API bool
deleteKeyAndSave(nlohmann::json& json, std::string const& filePath, std::vector<std::string> const& keyPath);

} // namespace GMLIB::Files::JsonFile