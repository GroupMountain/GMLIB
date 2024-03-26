#pragma once
#include "GMLIB/Macros.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Files::JsonFile {

GMLIB_API nlohmann::ordered_json initOrderedJson(std::string path, nlohmann::ordered_json& defaultFile);

GMLIB_API nlohmann::ordered_json initOrderedJson(std::string path, std::string& defaultFile);

GMLIB_API nlohmann::json initJson(std::string path, nlohmann::json& defaultFile);

GMLIB_API nlohmann::json initJson(std::string path, std::string& defaultFile);

GMLIB_API void writeOrUpdateOrderedFile(std::string path, nlohmann::ordered_json& defaultFile);

GMLIB_API void writeOrUpdateFile(std::string path, nlohmann::json& defaultFile);

GMLIB_API void updateOrderedFile(std::string& path, nlohmann::ordered_json& newFile);

GMLIB_API void updateOrderedFile(std::string& path, std::string& newFile);

GMLIB_API void updateFile(std::string& path, nlohmann::json& newFile);

GMLIB_API void updateFile(std::string& path, std::string& newFile);

GMLIB_API nlohmann::ordered_json readFromOrderedFile(std::string& path);

GMLIB_API nlohmann::json readFromFile(std::string& path);

GMLIB_API bool writeOrderedFile(std::string& path, nlohmann::ordered_json& json);

GMLIB_API bool writeFile(std::string& path, nlohmann::json& json);

template <typename T>
inline std::optional<T> getValue(nlohmann::ordered_json& json, std::vector<std::string> keyPath) {
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
inline std::optional<T> getValue(nlohmann::json& json, std::vector<std::string> keyPath) {
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
inline T getValue(nlohmann::ordered_json& json, std::vector<std::string> keyPath, T defaultValue) {
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
inline T getValue(nlohmann::json& json, std::vector<std::string> keyPath, T defaultValue) {
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
inline bool setValue(nlohmann::ordered_json& json, std::vector<std::string> keyPath, T data) {
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
inline bool setValue(nlohmann::json& json, std::vector<std::string> keyPath, T data) {
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
inline bool setValueAndSave(nlohmann::ordered_json& json, std::string& filePath, std::vector<std::string> keyPath, T data) {
    auto res = setValue<T>(json, keyPath, data);
    if (res) {
        return writeOrderedFile(filePath, json);
    }
    return false;
}

template <typename T>
inline bool setValueAndSave(nlohmann::json& json, std::string& filePath, std::vector<std::string> keyPath, T data) {
    auto res = setValue<T>(json, keyPath, data);
    if (res) {
        return writeFile(filePath, json);
    }
    return false;
}

GMLIB_API bool deleteOrderedKey(nlohmann::ordered_json& json, std::vector<std::string> keyPath);

GMLIB_API bool deleteKey(nlohmann::json& json, std::vector<std::string> keyPath);

GMLIB_API bool deleteOrderedKeyAndSave(nlohmann::ordered_json& json, std::string& filePath, std::vector<std::string> keyPath);

GMLIB_API bool deleteKeyAndSave(nlohmann::json& json, std::string& filePath, std::vector<std::string> keyPath);

} // namespace GMLIB::Files::JsonFile